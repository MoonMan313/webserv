//
// Created by Cheryle Dionna on 05/05/2022.
//

#ifndef PARSER_LOCATION_HPP
#define PARSER_LOCATION_HPP

#include <iostream>
#include <vector>
#include <map>
//
//#include "Location.hpp"
//#include "Server.hpp"
//#include "ParserConfig.hpp"
//
//#define DEFAULT_CONFIG "/home/evelina/Desktop/webserver/parser/default1.conf"

class Location {
protected:
    std::string                 methodsAllowed;
    bool                        autoindex;
    std::string                 index;
    std::vector<std::string>    cgi;
    std::string                 redirection;
    std::string                 root;
    unsigned int                limitBodySize;
    std::map<int, std::string>  errorPage;

public:
    void setMethodsAllowed(const std::string &methodsAllowed);

    void setAutoindex(bool autoindex);

    void setIndex(const std::string &index);

    void setCgi(const std::vector<std::string> &cgi);

    void setRedirection(const std::string &redirection);

    void setRoot(const std::string &root);

    void setLimitBodySize(unsigned int limitBodySize);

    void setErrorPage(char *first, char *second);

    const std::string &getMethodsAllowed() const;

    bool isAutoindex() const;

    const std::string &getIndex() const;

    const std::vector<std::string> &getCgi() const;

    const std::string &getRedirection() const;

    const std::string &getRoot() const;

    unsigned int getLimitBodySize() const;

    void getErrorPage() const;

public:
    Location();
};

#endif //PARSER_LOCATION_HPP
