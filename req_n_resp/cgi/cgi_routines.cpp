/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_routines.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 13:20:49 by gvolibea          #+#    #+#             */
/*   Updated: 2022/08/05 10:29:20 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <fstream>
void Cgi::execute(char **env)
{
	pid_t		pid;
	int			saved_out;
	std::string	temp;
	char		buffer [100];
	FILE		*fOut = tmpfile();
	int			fdOut = fileno(fOut);

	saved_out = dup(1);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (!pid)
	{
		dup2(fdOut, 1);
		if (execve(this->getRoot().c_str(), NULL, env) == -1)
			exit(EXIT_FAILURE); // may be add status (500 at moment);
		//close all nevessities
	}
	else
	{
		waitpid(pid, 0, 0);
		lseek(fdOut, 0, SEEK_SET);
		while (!feof (fOut))
		{
			if (fgets (buffer , 100 , fOut) == NULL)
				break;
			temp.append(buffer);
		}
		this->setResp(temp);
	}
	dup2(saved_out, 1);
	fclose(fOut);
	close(fdOut);
	int i = -1;
	while(env[++i])
		delete[] env[i];
	delete env;
}

int end_or_sep(std::string query)
{
	int len;

	if (query.find('&') != std::string::npos)
	{
		len = query.find('&');
	}
	else
	{
		len = query.length();
	}
	return (len);
};

std::map<std::string, std::string> parse_query(std::string query)
{
	std::map<std::string, std::string>	query_map;
	std::string							key;
	std::string							value;
	//parse query acc to its data, decode during request creation

	while(query.compare("") != 0)
	{

		key = query.substr(0, query.find('='));
		value = query.substr(query.find('=') + 1, \
			end_or_sep(query) - query.find('=') - 1);
		query_map[key] = value;
		query.erase(0, end_or_sep(query) + 1);
		std::cout << query << std::endl;
	}
	return (query_map);
};
