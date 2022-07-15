
#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <set>
#include <vector>
#include <iostream>
#include <stdlib.h>


class Server {
private:
    struct sockaddr_in                  _addr;
    int                                 _fd;
    int                                 _max_fd;
    

    std::vector<std::string>            serverName;
    
public:
    Server() {}
     int                                 read_fd;
     std::string                         msg_ret;
     ~Server() {}
     

    const int       getFd() {return _fd;}
    const int       getMaxFd() {return _max_fd;}
    void            setFd(int fd) {_fd = fd;}
    void            setAddr();
    struct sockaddr_in &getAddr() {return _addr;}
    void            setMaxFd(int mx_fd) {_max_fd = mx_fd;}
    const std::vector<std::string> &getServerName() const;


    int             setup();
    void            connect();
    void            close_fds();
};


#endif
