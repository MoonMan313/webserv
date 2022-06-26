#include "WebServer.hpp"

void    Server::setAddr()
{
    //memset(_addr.sin_zero, '\0', sizeof _addr.sin_zero);
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(8000/*getListens().port*/);
    _addr.sin_addr.s_addr = INADDR_ANY /*htonl(getListens().host)*/;
}

int     Server::setup()
{
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Socket error" << std::endl; /*throw(); must write exceptions*/
        return (-1);
    }

    this->setFd(fd);

    if (fcntl(this->getFd(), F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "FCNTL doesn't work" << std::endl; /*throw(); must write exceptions*/
        return (-1);
    } 

    this->setAddr();

    if (bind(this->getFd(), (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
    {
        std::cerr << "Bind error " <<  "." << std::endl;
        return (-1);
    }
    if (listen(this->getFd(), 1000) == -1)
    {
        std::cerr << "Listen error " << std::endl;
        return (-1);
    }
    return (0);
}

void    Server::connect()
{   
    char buf[30000] = {0};
    char buffer[30000] = {0};
    std::set<int> clients;
    
    clients.clear();
   

    while(1)
    {
        // Заполняем множество сокетов
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(this->getFd(), &readset);

        for(std::set<int>::iterator it = clients.begin(); it != clients.end(); it++)
            FD_SET(*it, &readset);


        // Задаём таймаут
        timeval timeout;
        timeout.tv_sec = 160; // was 15
        timeout.tv_usec = 0;
   
        int mx = std::max(this->getFd(), *max_element(clients.begin(), clients.end()));

 
        std::cout << "socket max is :  " << mx << std::endl;
        if(select(mx + 1, &readset, NULL, NULL, &timeout) <= 0)
        {
            perror("select");
            exit(3);
        }
        
        int addrlen = sizeof(this->getAddr());

        // Определяем тип события и выполняем соответствующие действия
        if(FD_ISSET(this->getFd(), &readset))
        {
            // Поступил новый запрос на соединение, используем accept
            int sock = accept(this->getFd(), (struct sockaddr *)&getAddr(), (socklen_t*)&addrlen);
            if(sock < 0)
            {
                perror("accept");
                exit(3);
            }
            
            std::cout << "socket_read is :  " << sock << std::endl;

            fcntl(sock, F_SETFL, O_NONBLOCK);

            clients.insert(sock);
            // vec_socks.push_back(sock);
        }

        for(std::set<int>::iterator it = clients.begin(); it != clients.end(); it++)

        {
            if(FD_ISSET(*it, &readset))
            {
                // Поступили данные от клиента, читаем их
                int bytes_read = recv(*it, buffer, 30000, 0);

                std:: cout << "bytes reads are: " << bytes_read << std::endl;
                if(bytes_read <= 0)
                {
                    // Соединение разорвано, удаляем сокет из множества
                    close(*it);
                    continue;
                }

                std::string hello = "HTTP/1.1. 200 OK\n\
	                                Content-Type: text\n\
	                                Content-Length: 12\n\n\
	                                <html>Hello big world!\n\
	                                <h>Header maybe zero</h>\
	                                <h1>Header maybe lruthann test</h1>\
                                    </div>\
                                    <div> <img src = '\test\test.png*'> </div>\
                                    </div>\
                                    <body>\
	                                </html>";
    std::string str = buffer;
		std::cout << str << std::endl << str.substr(0, str.find("\n")) << std::endl;

                // Отправляем данные обратно клиенту
                int send_bytes = send(*it, hello.c_str() , hello.length(), MSG_DONTROUTE);
                std:: cout << "send_bytes are: " << send_bytes << std::endl;
                if (send_bytes == -1)
                {
                     perror("send");
                    exit(3);
                }
            }
        }
    }
    

}

void    WebServer::close_fds()
{
    close(this->_fd);
    close(this->_max_fd);
    close(this->read_fd);
}
