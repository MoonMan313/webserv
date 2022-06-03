/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 18:10:11 by gvolibea          #+#    #+#             */
/*   Updated: 2022/06/01 18:25:44 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::Request() :  _port(80), _resp_status(0), _method(""), \
_path(""), _query("") {};

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

int Request::getPort() const
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
	percent_decoding(&path);
	this->_path = path;
};

void Request::setQuery(std::string query)
{
	percent_decoding(&query);
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

void Request::setPort(int port)
{
	this->_port = port;
};


void Request::setRespStatus(int rest_status)
{
	this->_resp_status = rest_status;
};

void Request::parse_request(std::string req)
{
	std::string first_line;

	//get first line
	first_line = get_first_line(req);
	if (first_line.empty())
	{
		this->setRespStatus(400);
		exit(EXIT_FAILURE);// develop here parsing stop procedure
	}
	//first line parsing
	first_line_parsing(first_line, this);
	req.erase(0, first_line.length() + 1);
	//get headers
	headers_parsing(req, this);
	//get message body
	debug_val(this->getPath());
	};
