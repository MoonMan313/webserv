/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 18:10:11 by gvolibea          #+#    #+#             */
/*   Updated: 2022/08/11 21:51:57 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::Request() :  _resp_status(200), _port("80"), _method(""), \
_path(""), _path_info(""), _body(NULL), _query("") {};

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

char *Request::getBody()
{
	return (this->_body);
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

void Request::setQuery(const char *body)
{
	std::string query (body);
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

void Request::setBody(char *body, int size)
{
	char body_proper[size];
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
	}

};

void Request::parse_request(char* req)
{
	std::string first_line;

 	first_line = get_first_line(req);
	std::cout << first_line << std::endl;
	first_line_parsing(first_line, this);
	//if (this->_resp_status != 200) return ;
	//req.erase(0, first_line.length() + 1);
	req = req + first_line.length() + 1;
	headers_parsing(req, this);
	if (this->getRespStatus() == 200)
		if (this->_body && this->getMethod() == "POST") // in case of PUT request -> check METHOD here
			this->setQuery(this->_body);
	};
