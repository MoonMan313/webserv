#include "response.hpp"

std::map<std::string, std::string>	get_headers(std::string headers_line, \
	Request *req)
{
	std::map<std::string, std::string>	headers;
	std::string							key;
	std::string							value;

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
		headers[key] = value;
		//std::cout << key << " : " << value << std::endl;
		headers_line.erase(0, end_of_line(headers_line) + 1);
	}
	if (headers.count("Content-Length") || headers.count("Transfer-Encoding"))
	{
		headers_line.erase(0, end_of_line(headers_line) + 1);
		req->setBody(headers_line.substr(1));
	}
	return headers;
}

//according to the rfc there should be no WS between first line
// and headers. If it is - such message shall be rejected
void headers_parsing(std::string headers_line, Request *req)
{
	/*std::map<std::string, std::string> headers;
	std::string key;
	std::string value;*/
	if (isspace(headers_line[0])) //if zero headers?
	{
		req->setRespStatus(400);
		return ;
	}
	req->setHeaders(get_headers(headers_line, req));
	if (req->getHeaders()["Host"].find(':') != std::string::npos)
	{
		req->setPort((req->getHeaders()["Host"]). \
			substr(req->getHeaders()["Host"].find(':') + 1));
	}
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
		req->setQuery(raw_line.substr(raw_line.find('?') + 1));
		raw_line.erase(raw_line.find('?'));
	}
	// check path
	if (!raw_line.empty())
		req->setPath(raw_line);
}

void check_fl_values(Request *req)
{
	std::string check_methods;

	check_methods = "GET_POST_DELETE_HEAD_PUT_CONNECT_OPTIONS_TRACE";
	//check if method is proper
	if (check_methods.find(req->getMethod()) == std::string::npos)
	{
		req->setRespStatus(400);
		return ;
	}
	//HTTP-name "/" DIGIT "." DIGIT
	if (req->getVersion().find("HTTP/") == std::string::npos)
		req->setRespStatus(400);
	else if (req->getVersion().length() != 8)
		req->setRespStatus(400);
	else if (req->getVersion().find(".") != 6)
		req->setRespStatus(400);
	else if (!isdigit(req->getVersion()[5]) || !isdigit(req->getVersion()[7]))
		req->setRespStatus(400);
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
	req->setMethod(s);
	if (temp.eof()) return;
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

std::string get_first_line(std::string req)
{
	std::string first_line;
	int pos;

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
