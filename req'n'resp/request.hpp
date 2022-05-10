/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmitriyp <dmitriyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:58:20 by dmitriyp          #+#    #+#             */
/*   Updated: 2022/05/10 18:18:42 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

class Request
{
private:
	std::string _method;
	std::string _version;
	std::string _path;
	std::string _query;
	std::map<std::string, std::string> _headers;
	std::string _port;
	std::string _encoding;
	std::string _accept;
	std::string _connection;
	std::string _language;
	int			_resp_status;
public:
	Request();
	~Request();

	std::string getMethod() const;
	std::string getVersion() const;
	std::string getPath() const;
	std::string getQuery() const;
	std::map<std::string, std::string> getHeaders() const;
	std::string getPort() const;
	int getStatus() const;

	void setMethod(std::string method);
	void setVersion(std::string version);
	void setPath(std::string path);
	void setQuery(std::string query);
	void setHeaders(std::map<std::string, std::string> headers);
	void setPort(std::string port);
	void setRespStatus(int rest_status);

	void parse_request(std::string req);
};

std::string get_first_line(std::string req);
void first_line_parsing(std::string f_line, Request req);
void headers_parsing(std::string headers_line, Request *req);
