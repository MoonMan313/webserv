//
// Created by Cheryle Dionna on 05/05/2022.
//

#include "Location.hpp"

Location::Location() {
    autoindex = 0;
    index = "";
    redirection = "";
    root = "";
    limitBodySize = 0;
    cgi = "";
}

bool Location::isAutoindex() const {
    return autoindex;
}

const std::string &Location::getIndex() const {
    return index;
}

const std::string &Location::getRedirection() const {
    return redirection;
}

std::string Location::getRoot() const {
    return root;
}

unsigned int Location::getLimitBodySize() const {
    return limitBodySize;
}

std::string Location::getErrorPage(int err_code)
{
    std::map <int, std::string>::const_iterator it;// = errorPage.begin();
	it = errorPage.find(err_code);
	if(it != errorPage.end())
	{
	    std::cout << "CHECK ERR PAGE" << it->first << " " << it->second << std::endl;
		return (it->second);
	}
	std::cout << err_code << " CHECK ERR PAGE FAILED" << std::endl;
	return (NULL);
}

void Location::setAutoindex(bool autoindex) {
    Location::autoindex = autoindex;
}

void Location::setIndex(const std::string &index) {
    Location::index = index;
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

const std::string &Location::getCgi() const {
    return cgi;
}

void Location::setCgi(const std::string &cgi) {
    Location::cgi = cgi;
}

void Location::setMethodsAllowed(const std::string &method) {
    methodsAllowed.push_back(method);
}

std::vector<std::string> Location::getMethodsAllowed() const {
    return this->methodsAllowed;
}
