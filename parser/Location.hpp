//
// Created by Cheryle Dionna on 05/05/2022.
//

#ifndef PARSER_LOCATION_HPP
#define PARSER_LOCATION_HPP

#include "ConfigUtils.hpp"

class Location {
private:
    std::string                 methodsAllowed;
    bool                        autoindex;
    std::string                 index;
    std::vector<std::string>    cgi;
    std::string                 redirection;
    std::string                 root;
    unsigned int                limitBodySize;
    std::map<int, std::string>  errorPage;
};

#endif //PARSER_LOCATION_HPP
