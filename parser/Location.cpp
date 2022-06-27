//
// Created by Cheryle Dionna on 05/05/2022.
//

#include <cstdlib>
#include "Location.hpp"

Location::Location() {

}

const std::string &Location::getMethodsAllowed() const {
    return methodsAllowed;
}

bool Location::isAutoindex() const {
    return autoindex;
}

const std::string &Location::getIndex() const {
    return index;
}

const std::vector<std::string> &Location::getCgi() const {
    return cgi;
}

const std::string &Location::getRedirection() const {
    return redirection;
}

const std::string &Location::getRoot() const {
    return root;
}

unsigned int Location::getLimitBodySize() const {
    return limitBodySize;
}

void Location::getErrorPage() const {
    std::map <int, std::string>::const_iterator it = errorPage.begin();
    for (; it != errorPage.end(); it++) {
        std::cout << it->first << " " << it->second << std::endl;
    }
}

void Location::setMethodsAllowed(const std::string &methodsAllowed) {
    Location::methodsAllowed = methodsAllowed;
}

void Location::setAutoindex(bool autoindex) {
    Location::autoindex = autoindex;
}

void Location::setIndex(const std::string &index) {
    Location::index = index;
}

void Location::setCgi(const std::vector<std::string> &cgi) {
    Location::cgi = cgi;
}

void Location::setRedirection(const std::string &redirection) {
    Location::redirection = redirection;
}

void Location::setRoot(const std::string &root) {
    Location::root = root;
}

void Location::setLimitBodySize(unsigned int limitBodySize) {
    Location::limitBodySize = limitBodySize;
}

void Location::setErrorPage(char *first, char *second) {
    Location::errorPage[atoi(first)] = second;
}
