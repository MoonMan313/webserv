/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvolibea <gvolibea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 16:52:00 by gvolibea          #+#    #+#             */
/*   Updated: 2022/07/05 15:12:48 by gvolibea         ###   ########.fr       */
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
#define PORT 8070

void send_image(void)
{
	Request req;
	Response resp;
	std::map<std::string, Location> locs;
	Location lc1;
	Location lc2;

	lc1.root = "";
	lc2.root = "temp";
	locs["/"] = lc1;
	locs["/path"] = lc2;

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
		std::cout << str << std::endl; //<< str.substr(0, str.find("\n")) << std::endl;
		// implement here reading of requests from browser
		req.parse_request(str);
		//resp.path_assembling_n_heck(locs, req);
		resp.make_response(locs, req);
        send(new_socket , resp.getRespons().c_str(), resp.getRespons().length(), MSG_DONTROUTE);
        printf("------------------Hello image sent-------------------");
		usleep(100000);
        close(new_socket);
    }
};

int main (void)
{
	send_image();
	return (0);
};
