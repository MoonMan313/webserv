/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:14:30 by gvolibea          #+#    #+#             */
/*   Updated: 2022/05/09 18:19:23 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int end_of_line(std::string str)
{
	int pos;
	
	pos = str.find("\n");
	if (pos == -1)
		pos = str.length();
	return (pos);
};

void trim_str(std::string* str)
{
	while (isspace((*str)[0]))
		str->erase(0,1);
	while (isspace((*str)[(*str).length() - 1]))
		(*str).pop_back();
};
