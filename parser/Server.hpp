//
// Created by Cheryle Dionna on 04/05/2022.
//

#ifndef PARSER_SERVER_HPP
#define PARSER_SERVER_HPP

#include <iostream>
#include <vector>
#include <map>

#include "Location.hpp"
#include "ConfigUtils.hpp"

class Server {
private:
//    std::string                         host;
//    std::string                         root;
    std::vector<std::string>            serverName;
    t_listen                		    listens;
    std::map<std::string, Location *>   location;
public:
    Server();

    const std::vector<std::string> &getServerName() const;

    const t_listen &getListens() const;

    const std::map<std::string, Location *> &getLocation() const;
};


#endif //PARSER_SERVER_HPP
