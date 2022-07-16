/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmitriyp <dmitriyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:58:20 by dmitriyp          #+#    #+#             */
/*   Updated: 2022/07/07 22:20:40 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

class Request
{
private:
	int									_resp_status;
	std::string							_port;
	std::string							_method;
	std::string							_version;
	std::string							_path;
	std::string							_path_info;
	std::string							_body;
	std::string							_query;
	std::string							_fragment;
	std::string							_encoding;
	std::string							_accept; //?
	std::string							_connection; //?
	std::string						 	_language;
	std::map<std::string, std::string>	_headers;
	std::map<std::string, std::string>	_headers_cgi;

public:
	Request();
	~Request();

	std::string							getMethod() const;
	std::string							getVersion() const;
	std::string							getPath() const;
	std::string							getPathInfo() const;
	std::string							getQuery() const;
	std::string							getFragment() const;
	std::string							getBody() const;
	std::map<std::string, std::string>	getHeaders() const;
	std::map<std::string, std::string>	getHeadersCgi() const;
	std::string							getPort() const;
	int									getRespStatus() const;
	//setters
	void								setMethod(std::string method);
	void								setVersion(std::string version);
	void								setPath(std::string path);
	void								setPathInfo(std::string path_info);
	void								setQuery(std::string query);
	void								setHeaders(std::map<std::string, \
		std::string> headers);
	void								setHeadersCgi(std::map<std::string, \
		std::string> headers);
	void								setPort(std::string port);
	void								setRespStatus(int rest_status);
	void								setFragment(std::string fragment);
	void								setBody(std::string body);

	void								parse_request(std::string req);
};

std::string								get_first_line(std::string req);
void									first_line_parsing(std::string f_line, \
	Request *req);
void									headers_parsing(std::string \
	headers_line, Request *req);
