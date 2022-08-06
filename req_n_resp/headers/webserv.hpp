/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmitriyp <dmitriyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 15:21:44 by dmitriyp          #+#    #+#             */
/*   Updated: 2022/08/06 17:39:21 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
//containers
#include <algorithm>
#include <map>
#include <stdio.h>
//parser part
#include <vector>
#include <cstdlib>
#include <cstring>
//for main
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <set>

//routine funcs
void trim_str(std::string *str);
int end_of_line(std::string str);
void percent_decoding(std::string *str);

//request response

//debugs
void debug_val(std::string str = "", int a = 0, char chr = ' ');
void debug_point(int a = 0);

#endif
