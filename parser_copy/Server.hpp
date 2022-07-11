//
// Created by Cheryle Dionna on 04/05/2022.
//

#ifndef PARSER_SERVER_HPP
#define PARSER_SERVER_HPP


#include "Location.hpp"
//#include "Server.hpp"
//#include "ParserConfig.hpp"

//#define DEFAULT_CONFIG "/home/evelina/Desktop/webserver/parser/default1.conf"

//class   Location;

class Server : public Location {
protected:
    std::string host;
    unsigned int port;
    std::string serverNames;

    std::map<std::string, Location *> location;
public:
    Location *getLocation(const char *path);

    Server();

    const std::string &getHost() const;

    unsigned int getPort() const;

    const std::string &getServerNames() const;
    void setHost(const std::string &host);

    void setPort(unsigned int port);

    void setServerNames(const std::string &serverNames);

    void setLocation(char *first, Location *second);


//
//    const std::vector<std::string> &getServerName() const;
//
//    const t_listen &getListens() const;
//
//    const std::map<std::string, Location *> &getLocation() const;
};


#endif //PARSER_SERVER_HPP
