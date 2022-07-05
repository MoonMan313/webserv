//
// Created by Cheryle Dionna on 05/05/2022.
//
#include <cstring>
#include <cstdlib>
#include "ParserConfig.hpp"

int parserLocation(char *word, Location *tmp) {
    if (tmp == NULL)
        throw "Error in configuration file.";
    else if (!strcmp(word, "autoindex") && tmp->isAutoindex() == 0) {
        word = strtok(NULL, " \t\v;");
        if (!strcmp(word, "on"))
            tmp->setAutoindex(1);
        else if (strcmp(word, "off"))
            throw "Error in configuration file.";
    } else if (!strcmp(word, "index") && tmp->getIndex() == "")
        tmp->setIndex(strtok(NULL, " \t\v;"));
    else if (!strcmp(word, "redirection") && tmp->getRedirection() == "")
        tmp->setRedirection(strtok(NULL, " \t\v;"));
    else if (!strcmp(word, "root") && tmp->getRoot() == "")
        tmp->setRoot(strtok(NULL, " \t\v;"));
    else if (!strcmp(word, "limit_body_size") && tmp->getLimitBodySize() == 0)
        tmp->setLimitBodySize(atoi(strtok(NULL, " \t\v;")));
    else if (!strcmp(word, "error_page")) {
        word = strtok(NULL, " \t\v;");
        tmp->setErrorPage(word, strtok(NULL, " \t\v;"));
    } else if (!strcmp(word, "cgi") && tmp->getCgi() == "")
        tmp->setCgi(strtok(NULL, " \t\v;"));
    else if (!strcmp(word, "methods_allowed")) {
        for (word = strtok(NULL, " \t\v"); strcmp(word, ";") && strcmp(word, ""); word = strtok(NULL, " \t\v")) {
            if (!strcmp(word, "GET"))
                tmp->setMethodsAllowed("GET");
            else if (!strcmp(word, "POST"))
                tmp->setMethodsAllowed("POST");
            else if (!strcmp(word, "DELETE"))
                tmp->setMethodsAllowed("DELETE");
            else
                throw "Error in configuration file.";
        }
    } else if (!strcmp(word, "}"))
        return 0;
    else
        throw "Error in configuration file.";
    return 1;
}

ParserConfig::ParserConfig(char *pathConfig) {
    const char *fileName;
    std::string line;
    std::string config;

    if (pathConfig)
        fileName = pathConfig;
    else
        fileName = DEFAULT_CONFIG;
    std::ifstream file(fileName);

    try {
        if (!file.is_open())
            throw "Failed to open configuration file.";
        while (std::getline(file, line)) {
            config += line + " ; ";
        }
        char *str = const_cast<char *>(config.c_str());
        char *word = std::strtok(str, " \t\v;");
        int i = -1;
        while (word != NULL) {
            if (!strcmp(word, "server") && !strcmp(strtok(NULL, " \t\v;"), "{")) {
                servers.push_back(new Server());
                ++i;
                word = strtok(NULL, " \t\v;");
                while (word != NULL) {
                    if (!strcmp(word, "host") && servers[i]->getHost() == "") {
                        servers[i]->setHost(strtok(NULL, " \t\v;"));
                    } else if (!strcmp(word, "port") && servers[i]->getPort() == 0) {
                        servers[i]->setPort(atoi(strtok(NULL, " \t\v;")));
                    } else if (!strcmp(word, "server_name") && servers[i]->getServerNames() == "") {
                        servers[i]->setServerNames(strtok(NULL, " \t\v;"));
                    } else if (parserLocation(word, servers[i])) {
                    } else if (!strcmp(word, "location")) {
                        char *path = strtok(NULL, " \t\v");
                        servers[i]->setLocation(path, new Location);
                        if (!strcmp(strtok(NULL, " \t\v;"), "{"))
                            while (parserLocation(strtok(NULL, " \t\v;"), servers[i]->getLocation(path))) {}
                        else
                            throw "Error in configuration file.";
                    } else if (!strcmp(word, "}")) {
                        break;
                    } else {
                        throw "Error in configuration file.";
                    }
                    word = strtok(NULL, " \t\v;");
                }
            } else
                throw "Error in configuration file.";

            word = strtok(NULL, " \t\v;");
        }
    }
    catch (const char *text_error) {
        std::cout << text_error << std::endl;
    }
    file.close();
}


//const std::vector<Server *> &ParserConfig::getServers() const {
//
//}
