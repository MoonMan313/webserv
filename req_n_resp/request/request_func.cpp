#include "response.hpp"

std::map<std::string, std::string> \
	cgi_headers(std::map<std::string, std::string> hdrs)
{
	std::map<std::string, std::string> cgi_headers;
	std::string temp;
	std::map<std::string, std::string>::iterator it;

	it = hdrs.begin();
	while(it != hdrs.end())
	{
		temp = it->first;
		while(temp.find('-') != std::string::npos)
			temp[temp.find('-')] = '_';
		temp = "HTTP_" + temp;
		//std::cout << "HEADER: " << it->first << "and CGI is :" << temp << std::endl;
		cgi_headers[temp] = it->second;
		it++;
	};
	return (cgi_headers);
};

std::map<std::string, std::string>	get_headers(char *headers_l, \
	Request *req)
{
	std::map<std::string, std::string>	headers;
	std::string							key;
	std::string							value;

	//while(headers_l[i] != '' || )
	int i = 0;
	while(headers_l[i])
	{
		if (headers_l[i] == '\r' && i > 1) //|| headers_l[i] == '\r')
			if (headers_l[i - 1] == '\n' && headers_l[i - 2] == '\r')
			{
			//	std::cout << "i for break : " << i << std::endl;
				break;
			}
		i++;
	}
	std::string	headers_line (headers_l, i + 1);
	while(headers_line.compare("") != 0 && headers_line.compare("\r\n\r\n") \
	!= 0 && headers_line[0] != '\r')
	{
		if (headers_line[headers_line.find(':') - 1]  == ' ' || \
		isspace(headers_line[0]) || headers_line.find(':') == std::string::npos)
		{
			req->setRespStatus(400);
			break;
		};
		key = headers_line.substr(0, headers_line.find(':'));
		value = headers_line.substr(headers_line.find(':') + 1, \
			end_of_line(headers_line) - headers_line.find(':'));
		trim_str(&value);
		//std::cout << "key: " << key << " and val: " << value << std::endl;
		headers[key] = value;
		headers_line.erase(0, end_of_line(headers_line) + 1);
	}
	if (headers.count("Content-Length") || headers.count("Transfer-Encoding"))
	{
		headers_l = headers_l + i + 2;
		//std::cout << "FINALLY BODY before IS:" << headers_l << "<-end" <<std::endl;

		int i = 0;
		while (i < std::stoi(headers["Content-Length"]) + 10)
		{
			std::cout << headers_l[i];
			i++;
		}
		if (headers_l)
			req->setBody(headers_l, std::stoi(headers["Content-Length"]));
	}
	return headers;
}

//according to the rfc there should be no WS between first line
// and headers. If it is - such message shall be rejected
void headers_parsing(char *headers_line, Request *req)
{
	if (isspace(headers_line[0])) //if zero headers?
		req->setRespStatus(400); //moved return from here to try to recogmnize host and port
	req->setHeaders(get_headers(headers_line, req));
	req->setHeadersCgi(cgi_headers(req->getHeaders()));
	if (req->getHeaders()["Host"].find(':') != std::string::npos)
	{
		req->setPort((req->getHeaders()["Host"]). \
			substr(req->getHeaders()["Host"].find(':') + 1));
		req->setHost(req->getHeaders()["Host"].substr(0, req->getHeaders()["Host"].find(":")));
	}
	else
		req->setHost(req->getHeaders()["Host"]);
}

void parse_path(Request *req, std::string raw_line)
{
	// check fragment
	if (raw_line.find('#') != std::string::npos)
	{
		req->setFragment(raw_line.substr(raw_line.find('#') + 1));
		raw_line.erase(raw_line.find('#'));
	}
	// check query
	if (raw_line.find('?') != std::string::npos)
	{
		req->setQuery((raw_line.substr(raw_line.find('?') + 1)).c_str());
		raw_line.erase(raw_line.find('?'));
	}
	// check path_info
	if (raw_line.find(".cgi") != std::string::npos)
	{
		req->setPathInfo(raw_line.substr(raw_line.find(".cgi") + 4));
		std::cout << "PATH INFO :" << req->getPathInfo() << std::endl;
		raw_line.erase(raw_line.find(".cgi") + 4);
	}
	if (!raw_line.empty())
		req->setPath(raw_line);
}

void check_fl_values(Request *req)
{
	//HTTP-name "/" DIGIT "." DIGIT
	if (req->getRespStatus() == 200)
	{
		if (req->getVersion().find("HTTP/") == std::string::npos)
			req->setRespStatus(400);
		else if (req->getVersion().length() != 8)
			req->setRespStatus(400);
		else if (req->getVersion().find(".") != 6)
			req->setRespStatus(400);
		else if (!isdigit(req->getVersion()[5]) || !isdigit(req->getVersion()[7]))
			req->setRespStatus(400);
		else if (req->getVersion()[5] != '0' && req->getVersion()[5] != '1')
			req->setRespStatus(505);
	}
};

//according to rfc normal request first line should be splitted by WS
//in case of wrong encoding made by user agent WS could be present
//inside of field of first f_line. Proper status should follow
void first_line_parsing(std::string f_line, Request *req)
{
	std::stringstream temp;
	std::string s;

	req->setRespStatus(400);
	temp << f_line;
	getline(temp, s, ' ');
	if (temp.eof() || !(req->setMethod(s))) return;
	getline(temp, s, ' ');
	if (temp.eof()) return;
	parse_path(req, s);
	getline(temp, s, '\r');
	req->setVersion(s);
	getline(temp, s);
	if (!(s.length() == 0)) return;
	req->setRespStatus(200);
	check_fl_values(req);
}

std::string get_first_line(char *req_line)
{
	std::string first_line;
	int i;
	int pos;

	i = 0;
	while(req_line[i] != '\n' && req_line[i])
		i++;
	std::string req (req_line, i + 1);
	//status line end shall be CRLF = /r/n (check w readl web request)
	pos = req.find('\n');
	if (pos == -1)
		pos = req.length();
	if (pos == 0)
		first_line.clear();
	if (pos != 0)
		first_line = req.substr(0, pos);
	return first_line;
}
