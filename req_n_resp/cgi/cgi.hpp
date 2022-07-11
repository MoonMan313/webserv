/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 13:03:06 by gvolibea          #+#    #+#             */
/*   Updated: 2022/07/10 13:13:46 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include "request.hpp"

class Cgi
{
private:
	std::map<std::string, std::string>	_env;
	std::string							_resp_body;
	std::string							_root;
	int									_resp_status;
public:
	Cgi(std::string root);
	~Cgi();

	void execute(char **env);
	void executeCgi(Request req);
	void make_def_env(Request req);
	char **convert_env(void);
	void add_req_hearders(Request req);

	//setters
	void setQueryVars(Request req);
	void setResp(std::string str);
	void setRespStatus(int status);

	//getters
	std::map<std::string, std::string>	getEnv(void);
	std::string							getResp(void);
	std::string 						getRoot(void);
	int									getRespStatus(void);
};

std::map<std::string, std::string> parse_query(std::string query);
std::string file_to_str(std::ifstream is);


#endif
