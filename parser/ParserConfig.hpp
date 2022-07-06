//
// Created by Cheryle Dionna on 05/05/2022.
//

#ifndef PARSER_PARSERCONFIG_HPP
#define PARSER_PARSERCONFIG_HPP

#include <iostream>
#include <vector>
#include <map>

//#include "Location.hpp"
#include "Server.hpp"

#define DEFAULT_CONFIG "/home/evelina/Desktop/webserver/parser/default.conf"
#include <fstream>

class   Server;

class ParserConfig {
private:
    std::vector<Server *>   servers;

public:
    ParserConfig(char *pathConfig);

    const std::vector<Server *> &getServers() const;
};


#endif //PARSER_PARSERCONFIG_HPP
