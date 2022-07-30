//
// Created by Cheryle Dionna on 05/05/2022.
//

#ifndef PARSER_PARSERCONFIG_HPP
#define PARSER_PARSERCONFIG_HPP

#include "Server.hpp"

#define DEFAULT_CONFIG "website/parser_copy/default.conf"

class ParserConfig {
private:
    std::vector<Server *>   servers;

public:
    ParserConfig(char *pathConfig);

    std::vector<Server *> getServers() const;
};


#endif //PARSER_PARSERCONFIG_HPP
