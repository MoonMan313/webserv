/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 18:10:11 by gvolibea          #+#    #+#             */
/*   Updated: 2022/07/05 10:44:05 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::Request() :  _resp_status(200), _port("80"), _method(""), \
_path(""), _body(""), _query("") {};

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

std::string	Request::getPort() const
{
	return (this->_port);
};

int Request::getRespStatus() const
{
	return (this->_resp_status);
};

std::string Request::getBody() const
{
	return (this->_body);
};

//SETTERS
void Request::setMethod(std::string method)
{
	this->_method = method;
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

void Request::setQuery(std::string query)
{
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

void Request::setPort(std::string port)
{
	this->_port = port;
};

void Request::setRespStatus(int rest_status)
{
	this->_resp_status = rest_status;
};

void Request::setBody(std::string body)
{
	this->_body = body;
};

void Request::parse_request(std::string req)
{
	std::string first_line;


	//get first line
	first_line = get_first_line(req);
	//first line parsin
	std::cout << "FL is :" << first_line << std::endl;
	first_line_parsing(first_line, this);
	if (this->_resp_status == 400) return;
	req.erase(0, first_line.length() + 1);
	//get headers
	headers_parsing(req, this);
	};
