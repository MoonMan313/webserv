//
// Created by Cheryle Dionna on 04/05/2022.
//

#ifndef PARSER_SERVER_HPP
#define PARSER_SERVER_HPP

#include "ConfigUtils.hpp"

class Server : public Location {
private:
    std::string                         host;
    unsigned int                        port;
    std::string                         serverNames;

    std::map<std::string, Location *>   location;
public:
//    Server();
//
//    const std::vector<std::string> &getServerName() const;
//
//    const t_listen &getListens() const;
//
//    const std::map<std::string, Location *> &getLocation() const;
};


#endif //PARSER_SERVER_HPP
