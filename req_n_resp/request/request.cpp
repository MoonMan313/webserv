/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 18:10:11 by gvolibea          #+#    #+#             */
/*   Updated: 2022/08/14 12:18:57 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::Request() :  _resp_status(200), _port("80"), _method(""), \
_path(""), _path_info(""), _body(""), _query("") {};

Request::~Request(){};

std::string Request::getMethod() const
{
	return (this->_method);
};

std::string Request::getVersion() const
{
	return (this->_version);
};

std::string Request::getPath() const
{
	return (this->_path);
};

std::string Request::getPathInfo() const
{
	return (this->_path_info);
};

std::string Request::getQuery() const
{
	return (this->_query);
};

std::string Request::getFragment() const
{
	return (this->_fragment);
};

std::map<std::string, std::string> Request::getHeaders() const
{
	return (this->_headers);
};

std::map<std::string, std::string> Request::getHeadersCgi() const
{
	return (this->_headers_cgi);
};

std::string	Request::getPort() const
{
	return (this->_port);
};

std::string	Request::getHost() const
{
	return (this->_host);
};

int Request::getRespStatus() const
{
	return (this->_resp_status);
};

std::string Request::getBody()
{
	return (this->_body);
};

std::vector<char> Request::getInputVec(void)
{
	return (this->_input_vec);
};

std::vector<char> Request::getBodyPut(void)
{
	return (this->_body_put);
};


//SETTERS
int Request::setMethod(std::string method)
{
	std::string check_methods;

	check_methods = "GET_POST_DELETE_HEAD_PUT_CONNECT_OPTIONS_TRACE";
	if (check_methods.find(method) == std::string::npos || method == " " || method == "")
		return 0;
	this->_method = method;
	return 1;
};

void Request::setVersion(std::string version)
{
	this->_version = version;
};

void Request::setPath(std::string path)
{
	percent_decoding(&path);
	this->_path = path;
};

void Request::setPathInfo(std::string path_info)
{
	this->_path_info = path_info;
};

void Request::setQuery(std::string query)
{
	//std::string query (body);
	percent_decoding(&query); // may be failure to devode here as equeal sign could appear inside credentials etc
	this->_query = query;
};

void Request::setFragment(std::string fragment)
{
	percent_decoding(&fragment);
	this->_fragment = fragment;
};


void Request::setHeaders(std::map<std::string, std::string> headers)
{
	this->_headers = headers;
};

void Request::setHeadersCgi(std::map<std::string, std::string> headers)
{
	this->_headers_cgi = headers;
};

void Request::setPort(std::string port)
{
	this->_port = port;
};

void Request::setHost(std::string host)
{
	this->_host = host;
};

void Request::setRespStatus(int rest_status)
{
	this->_resp_status = rest_status;
};

void Request::setInputVec(std::vector<char> vec)
{
	this->_input_vec = vec;
}

void Request::setBodyPut(std::vector<char> vec)
{
	this->_body_put = vec;
};

void Request::setBody(std::string body)//, int size)
{

	this->_body = body;
	/*char body_proper[size];
	int i = 0;
	while (i < size)
	{
		body_proper[i] = body[i];
		i++;
	}
	//body_proper[i] = '\0';
	i = 0;
	this->_body = body_proper;

	while (i < size)
	{
		std::cout << this->_body[i];
		i++;
	}*/

};

void Request::make_put_body(int body_start, std::vector<char> input_vec)
{
	std::cout << "body start pos :" << input_vec[body_start] << std::endl;
	input_vec.erase(input_vec.begin(), input_vec.begin() + body_start);
	if (!input_vec.empty())
		this->setBodyPut(input_vec);
}


void Request::parse_request(std::string req, std::vector<char> input_vec)//char* req)
{
	std::string first_line;

	if (req.find('\n') == 0)
		req.erase(0, 1);
	first_line = req.substr(0, req.find("\r\n") + 1);
	first_line_parsing(first_line, this);
	if (this->getMethod() == "PUT")
		make_put_body(req.find("\r\n\r\n") + 4, input_vec);
	req = req.erase(0, first_line.length() + 1);
	headers_parsing(req, this);
	if (this->getRespStatus() == 200)
		if (this->_body != "" && this->getMethod() == "POST") // in case of PUT request -> check METHOD here
			this->setQuery(this->_body);
	};
