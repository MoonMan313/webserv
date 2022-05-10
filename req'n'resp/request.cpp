/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 18:10:11 by gvolibea          #+#    #+#             */
/*   Updated: 2022/05/10 18:18:46 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::Request() : _resp_status(0){};

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

std::map<std::string, std::string> Request::getHeaders() const
{
	return (this->_headers);
};

std::string Request::getPort() const
{
	return (this->_port);
};

int Request::getStatus() const
{
	return (this->_resp_status);
};

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
	this->_path = path;
};

void Request::setQuery(std::string query)
{
	this->_query = query;
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

bool space_predicate(int a)
{
	if (std::isspace(a))
		return true;
	return (false);
};

void Request::parse_request(std::string req)
{
	//get first line
	std::string first_line;

	first_line = get_first_line(req);
	if (first_line.empty())
	{
		this->setRespStatus(400);
		exit(EXIT_FAILURE);// develop here parsing stop procedure
	}

	//first line parsing
	first_line_parsing(first_line, *this);
	std::cout << this->getStatus() << "this is status" << std::endl;
	req.erase(0, first_line.length() + 1);
	//get headers
	headers_parsing(req, this);
	//get message body
	std::cout << this->getStatus() << " this is status" << std::endl;
};
