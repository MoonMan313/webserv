//
// Created by Cheryle Dionna on 04/05/2022.
//

#ifndef PARSER_SERVER_HPP
#define PARSER_SERVER_HPP

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

// #include "Location.hpp"
// #include "ConfigUtils.hpp"

class Server {
private:
     
//    std::string                         host;
//    std::string                         root;
    
    struct sockaddr_in                  _addr;
    int                                 _fd;
    int                                 _max_fd;
    

    std::vector<std::string>            serverName;
    // t_listen                		    _listens;
    // std::map<std::string, Location *>   location;
    
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

    // const t_listen &getListens() const;

    // const std::map<std::string, Location *> &getLocation() const;

    int             setup();
    void            connect();
    void            close_fds();
};


#endif //PARSER_SERVER_HPP
