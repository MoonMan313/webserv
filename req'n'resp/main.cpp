/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 16:52:00 by gvolibea          #+#    #+#             */
/*   Updated: 2022/06/04 00:11:33 by gvolibea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <fcntl.h>

#include "response.hpp"
//#include "Location.hpp"
#define PORT 8080

void send_image(Response &resp)
{
    int server_fd, new_socket;
	//long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        read( new_socket , buffer, 30000);
		std::string str = buffer;
		std::cout << str << std::endl << str.substr(0, str.find("\n")) << std::endl;
        send(new_socket , resp.getRespons().c_str(), resp.getRespons().length(), MSG_DONTROUTE);
        printf("------------------Hello image sent-------------------");
		usleep(100000);
        close(new_socket);
    }
};

int main (void)
{
	std::string str;
	Request req;
	Response resp;
	Location lc1;
	Location lc2;
//	Location lc2;
	std::map<std::string, Location> locs;

//	lc1 = Location();
//	lc2 = Location();

	str = "GET /test.jpg HTTP/1.1\nHost: localhost:8080\nContent-Info:ss\n\n";
	std::cout << "location index is " << lc1.get_index() << std::endl;
	req.parse_request(str);
	lc1.root = "/";
	lc2.root = "test_folder";
	locs["/"] = lc1;
	locs["path"] = lc2;
	//resp.path_assembling_n_check(locs, req);
	resp.make_response(locs, req);
	send_image(resp);
	return (0);
};
