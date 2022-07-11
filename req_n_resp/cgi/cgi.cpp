/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 13:03:10 by gvolibea          #+#    #+#             */
/*   Updated: 2022/07/10 13:20:13 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

Cgi::Cgi(std::string root) : _resp_body(""), _root(root) {};

Cgi::~Cgi(){};

char **Cgi::convert_env(void)
{
	std::map<std::string, std::string> temp;
	std::map<std::string, std::string>::iterator it;
	std::string temp_str;
	int i;

	temp = this->getEnv();
	char	**env = new char*[temp.size() + 1]; // add failure check
	it = temp.begin();
	i = 0;
	while(it != temp.end())
	{
		temp_str.append(it->first);
		temp_str.append("=");
		temp_str.append(it->second);
		env[i] = new char[temp_str.size() + 1];
		env[i] = strcpy(env[i],(const char*)temp_str.c_str());
		temp_str = "";
		it++;
		i++;
	}
	env[i] = NULL;
	return (env);
};

void Cgi::make_def_env(Request req)
{
	this->_env["REDIRECT_STATUS"] = "200";
	this->_env["SCRIPT_NAME"] = req.getPath();
	this->_env["REQUEST_METHOD"] = req.getMethod();
	this->_env["CONTENT_LENGTH"] = req.getHeaders()["Content-Length"];
	this->_env["CONTENT_TYPE"] = req.getHeaders()["Accept"]; //specify content type which were mentioned in req (accept field is applied here)
	this->_env["PATH_INFO"] = req.getPathInfo(); //wtf is that
	this->_env["PATH_TRANSLATED"] = this->getRoot(); //might need some change, using config path/contentLocation
	this->_env["QUERY_STRING"] = req.getQuery();
	this->_env["REMOTE_ADDR"] = req.getHeaders()["Host"];
	//this->_env["REMOTE_USER"] = headers["Authorization"]; //user data ident
	this->_env["REQUEST_URI"] =  this->getRoot();// + "?" + req.getQuery();
	//if (headers.find("Hostname") != headers.end())
	this->_env["SERVER_NAME"]  = req.getHeaders()["Host"].substr(0, \
	req.getHeaders()["Host"].find(":"));
	/*else
		this->_env["SERVER_NAME"] = this->_env["REMOTEaddr"];*/
	this->_env["SERVER_PORT"] = req.getPort();
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_SOFTWARE"] = "Weebserv/1.0";
	this->_env["HOST"] = req.getHeaders()["Host"].substr(0, \
	req.getHeaders()["Host"].find(":"));
// HTTP_REFERER  - available in request
}

void Cgi::add_req_hearders(Request req)
{
	std::map<std::string, std::string> cgi_headers;
	std::map<std::string, std::string>::iterator it;

	cgi_headers = req.getHeadersCgi();
	it = cgi_headers.begin();
	while(it != cgi_headers.end())
	{
		this->_env[it->first] = it->second;
		std::cout << "I AND: " << it->first << " AND " << it->second << std::endl;
		it++;
	};
};

void Cgi::executeCgi(Request req)
{
	char **env;
	//get vars from QUERY_STRING
	this->setQueryVars(req);
	//init env
	std::cout << "check query: CHECK2"<< std::endl;
	make_def_env(req);
	add_req_hearders(req);
	// convert envs to cstr
	env = convert_env();
	//executeCgi
	execute(env);
}

void Cgi::setQueryVars(Request req)
{
	this->_env = parse_query(req.getQuery());
}
void Cgi::setResp(std::string str)
{
	this->_resp_body = str;
}

void Cgi::setRespStatus(int status)
{
	this->_resp_status = status;
};

std::map<std::string, std::string>	Cgi::getEnv(void)
{
	return (this->_env);
}

std::string	Cgi::getRoot(void)
{
	return (this->_root);
};

std::string Cgi::getResp(void)
{
	return (this->_resp_body);
};

int Cgi::getRespStatus(void)
{
	return (this->_resp_status);
};
