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
    std::vector<std::string>    methodsAllowed;
    bool                        autoindex;
    std::string                 index;
    std::string                 cgi;
    std::string                 redirection;
    std::string                 root;
    unsigned int                limitBodySize;
    std::map<int, std::string>  errorPage;
public:
    const std::string &getCgi() const;

    void setCgi(const std::string &cgi);

public:

    void setAutoindex(bool autoindex);

    void setMethodsAllowed(const std::string &method) ;

    void setIndex(const std::string &index);

    void setRedirection(const std::string &redirection);

    void setRoot(const std::string &root);

    void setLimitBodySize(unsigned int limitBodySize);

    void setErrorPage(char *first, char *second);

    bool isAutoindex() const;

    const std::string &getIndex() const;

    const std::string &getRedirection() const;

    void  getMethodsAllowed() const;

    const std::string &getRoot() const;

    unsigned int getLimitBodySize() const;

    void getErrorPage() const;

public:
    Location();
};

#endif //PARSER_LOCATION_HPP
