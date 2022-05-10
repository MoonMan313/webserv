#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
	long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Only this line has been changed. Everything is same.
    std::string hello = "HTTP/1.1 200 OK\n\
	Content-Type: text\n\
	Content-Length: 12\n\n\
	<html>Hello big world!\n\
	<h>Header maybe zero</h>\
	<h1>Header maybe one</h1>\
	</html>";

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
        valread = read( new_socket , buffer, 30000);
		std::string str = buffer;
		std::cout << str << std::endl << str.substr(0, str.find("\n")) << std::endl;
        send(new_socket , hello.c_str() , hello.length(), MSG_DONTROUTE);
        printf("------------------Hello message sent-------------------");
		usleep(100000);
        close(new_socket);
    }
    return 0;
}
