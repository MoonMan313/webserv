//
// Created by Cheryle Dionna on 04/05/2022.
//

#include "Server.hpp"

//парсер параметров server до "}"
Server::Server() {

}

const std::vector<std::string> &Server::getServerName() const {
    return serverName;
}

const t_listen &Server::getListens() const {
    return listens;
}

const std::map<std::string, Location *> &Server::getLocation() const {
    return location;
}

