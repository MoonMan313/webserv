#include "request.hpp"
#include "response.hpp"

std::map<std::string, std::string> get_headers(std::string headers_line, \
	Request *req)
{
	std::map<std::string, std::string> headers;
	std::string key;
	std::string value;

	int i = 0;
	while(headers_line.compare("") != 0 && headers_line.compare("\n") != 0)
	{
		if (headers_line[headers_line.find(':') - 1]  == ' ' || \
		isspace(headers_line[0]) || headers_line.find(':') == -1)
		{
			req->setRespStatus(400);
			break;
		};
		key = headers_line.substr(0, headers_line.find(':'));
		value = headers_line.substr(headers_line.find(':') + 1, \
			end_of_line(headers_line) - headers_line.find(':'));
		trim_str(&value);
		headers[key] = value;
		headers_line.erase(0, end_of_line(headers_line) + 1);
		//std::cout << "last part 1 of headers is X" << headers_line << "X" << std::endl;
		if (i++ == 4) //temp counter
			break;
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
}

//according to rfc normal request first line should be splitted by WS
//in case of wrong encoding made by user agent WS could be present
//	inside of field of first f_line. Proper status should follow
void first_line_parsing(std::string f_line, Request req)
{
	std::stringstream temp;
	std::string s;

	temp << f_line;
	getline(temp, s, ' ');
	req.setMethod(s);
	getline(temp, s, ' ');
	req.setPath(s);
	getline(temp, s, ' ');
	req.setVersion(s);
	temp >> s;
	if (s.compare(req.getVersion()) != 0)
		req.setRespStatus(400);
};

std::string get_first_line(std::string req)
{
	std::string first_line;
	int pos;

	pos = req.find('\n');
	if (pos == -1)
		pos = req.length();
	if (pos == 0)
		first_line.clear();
	if (pos != 0)
		first_line = req.substr(0, pos);
	return first_line;
}
