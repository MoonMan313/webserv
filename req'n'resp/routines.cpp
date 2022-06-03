/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:14:30 by gvolibea          #+#    #+#             */
/*   Updated: 2022/06/01 15:51:49 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// percent decoding
void percent_decoding(std::string *str)
{
	char *pend;
	char incl;
	std::string hexi = "0x";

	while ((*str).find('%') != std::string::npos)
	{
		incl = static_cast<char>(strtol((hexi + (*str).substr((*str).find('%') + \
		1, 2)).c_str(), &pend, 16));
		(*str).insert((*str).find('%'), &incl, 1);
		(*str).erase((*str).find('%'), 3);
	}
	debug_val(*str);
};

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


//debugs
void debug_val(std::string str, int a, char chr)
{
	std::cout << "DEBUG START" << std::endl;
	if (!str.empty())
		std::cout << "\ncheck str_start_next_line ->\n" << str  << "\n<- str_end_prev_line\n"<< std::endl;
	if (a)
		std::cout << "checl int: " << a << std::endl;
	if (chr != ' ')
		std::cout << "checl chr: " << chr << std::endl;
	std::cout << "DEBUG END" << std::endl;
};

void debug_point(int a)
{
	if (!a)
		std::cout << "DEBUG POINT" << std::endl;
	else
		std::cout << "DEBUG POINT #" << a << std::endl;
};
