/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmitriyp <dmitriyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 15:21:44 by dmitriyp          #+#    #+#             */
/*   Updated: 2022/06/01 17:04:48 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
#include <sstream>
#include <fstream>
//containers
#include <algorithm>
#include <map>



//routine funcs
void trim_str(std::string *str);
int end_of_line(std::string str);
void percent_decoding(std::string *str);

//request response

//debugs
void debug_val(std::string str = "", int a = 0, char chr = ' ');
void debug_point(int a = 0);
