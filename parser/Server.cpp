//
// Created by Cheryle Dionna on 04/05/2022.
//

#include "Server.hpp"

Server::Server() {
    host = "";
    port = 0;
    serverNames = "";
    methodsAllowed = "";
    autoindex = 0;
    index = "";
    redirection = "";
    root = "";
    limitBodySize = 0;
}

const std::string &Server::getHost() const {
    return host;
}

unsigned int Server::getPort() const {
    return port;
}

const std::string &Server::getServerNames() const {
    return serverNames;
}

void Server::setHost(const std::string &host) {
    Server::host = host;
}

void Server::setPort(unsigned int port) {
    Server::port = port;
}

void Server::setServerNames(const std::string &serverNames) {
    Server::serverNames = serverNames;
}

void Server::setLocation(const std::map<std::string, Location *> &location) {
    Server::location = location;
}

const std::map<std::string, Location *> &Server::getLocation() const {
    return location;
}

//const std::vector<std::string> &Server::getServerName() const {
//    return serverName;
//}
//
//const t_listen &Server::getListens() const {
//    return listens;
//}
//
//const std::map<std::string, Location *> &Server::getLocation() const {
//    return location;
//}

