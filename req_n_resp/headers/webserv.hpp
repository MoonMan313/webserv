/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmitriyp <dmitriyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 15:21:44 by dmitriyp          #+#    #+#             */
/*   Updated: 2022/07/10 20:27:56 by gvolibea         ###   ########.fr       */
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


//routine funcs
void trim_str(std::string *str);
int end_of_line(std::string str);
void percent_decoding(std::string *str);

//request response

//debugs
void debug_val(std::string str = "", int a = 0, char chr = ' ');
void debug_point(int a = 0);

#endif
