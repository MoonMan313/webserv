
#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include "response.hpp"

class WebServer {
private:
    struct sockaddr_in                  _addr;
    int                                 _fd;
    int                                 _max_fd;


    std::vector<std::string>            serverName;

public:
    WebServer() {}
     int                                 read_fd;
     std::string                         msg_ret;
     ~WebServer(){}


    int       getFd() {return _fd;}
    int       getMaxFd() {return _max_fd;}
    void            setFd(int fd) {_fd = fd;}
    void            setAddr();
    struct sockaddr_in &getAddr() {return _addr;}
    void            setMaxFd(int mx_fd) {_max_fd = mx_fd;}
    const std::vector<std::string> &getServerName() const;


    int             setup();
    void            connect(ParserConfig config);
    void            close_fds();
};


#endif
