/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 16:52:00 by gvolibea          #+#    #+#             */
/*   Updated: 2022/05/10 18:25:17 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

int main (void)
{
	std::string str;
	Request req;

	str = "GET / HTTP/1.1\nHost: localhost:8080\nContent-Info:ss\n\n";
	req.parse_request(str);
	//std::cout << "meth is: " << req.getMethod() << " ver is:" << req.getVersion();
//	std::string test_me = "  trim trrim   ";
//	trim_str(&test_me);
//	std::cout << "start:" << test_me << ":->end";
	return (0);
};
