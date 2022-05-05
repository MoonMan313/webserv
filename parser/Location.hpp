//
// Created by Cheryle Dionna on 05/05/2022.
//

#ifndef PARSER_LOCATION_HPP
#define PARSER_LOCATION_HPP

#include "Server.hpp"

class Location {
private:
    std::string                 root;
    std::string                 index;
    std::string                 url;
    bool                        autoindex;
    std::vector<std::string>    cgi;
};


#endif //PARSER_LOCATION_HPP
