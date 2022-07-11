#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <fcntl.h>
//#include <io.h>

#define PORT 8080

std::string get_file_data()
{
//	std::string out;
	std::ifstream is ("test.jpg", std::ifstream::binary);
	std::stringstream ss;
	std::string out;
	if (is) {
		// get length of file:
		is.seekg (0, is.end);
		int length = is.tellg();
		is.seekg (0, is.beg);
		char *buffer = new char[length];
		// read data as a block:
		is.read (buffer,length);
		int i = -1;
		ss.write(buffer, length);
		out = ss.str();
		is.close();
		// ...buffer contains the entire file...
		delete[] buffer;
	}
	return (out);
};

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
	long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
	std::string file_str;

	file_str = get_file_data();
    // Only this line has been changed. Everything is same.

	std::string file_size_str = std::to_string(file_str.length());

	std::string hello = "HTTP/1.1 200 Okay\r\nContent-Transfer-Encoding: binary; Content-Length: " + file_size_str + ";charset=ISO-8859-4 \r\n\r\n" + file_str;


   // std::string hello = "HTTP/1.1 200 OK\n\
	Content-Type: image/jpg\n\
	Content-Length: 242068\n\n\
	<html> \
	 <head> \
	 <title>Ссылка</title> \
	 </head> \
	 <body> \
	  <b>GHSFS<\b>\
	  <img src=\"./test.jpg\" width=\"150\" \
	  height=\"150\" alt=\"SAPLE\">\
	 </body> \
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
