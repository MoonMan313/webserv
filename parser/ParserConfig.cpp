//
// Created by Cheryle Dionna on 05/05/2022.
//
#include <cstring>
#include <cstdlib>
#include "ParserConfig.hpp"

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
            config += ' ' + line;
        }
        char *str = const_cast<char *>(config.c_str());
        char *word = std::strtok(str, " \t\v");
        int i = -1;

        while (word != NULL) {
            if (!strcmp(word, "server") && !strcmp(strtok(NULL, " \t\v"), "{")) {
                servers.push_back(new Server());
                ++i;
                word = strtok(NULL, " \t\v");
                while (word != NULL) {
                    if (!strcmp(word, "host") && servers[i]->getHost() == "") {
                        servers[i]->setHost(strtok(NULL, " \t\v"));
//                        std::cout << servers[i]->getHost() << std::endl;
                    } else if (!strcmp(word, "port") && servers[i]->getPort() == 0) {
                        servers[i]->setPort(atoi(strtok(NULL, " \t\v")));
//                        std::cout << servers[i]->getPort() << std::endl;
                    } else if (!strcmp(word, "server_name") && servers[i]->getServerNames() == "") {
                        servers[i]->setServerNames(strtok(NULL, " \t\v"));
//                        std::cout << servers[i]->getServerNames() << std::endl;
                    } else if (!strcmp(word, "autoindex") && servers[i]->isAutoindex() == 0) {
                        word = strtok(NULL, " \t\v");
                        if (!strcmp(word, "on"))
                            servers[i]->setAutoindex(1);
                        else if (strcmp(word, "off"))
                            throw "Error in configuration file.";
//                        std::cout << servers[i]->isAutoindex() << std::endl;
                    } else if (!strcmp(word, "index") && servers[i]->getIndex() == "") {
                        servers[i]->setIndex(strtok(NULL, " \t\v"));
//                        std::cout << servers[i]->getIndex() << std::endl;
                    } else if (!strcmp(word, "redirection") && servers[i]->getRedirection() == "") {
                        servers[i]->setRedirection(strtok(NULL, " \t\v"));
//                        std::cout << servers[i]->getRedirection() << std::endl;
                    } else if (!strcmp(word, "root") && servers[i]->getRoot() == "") {
                        servers[i]->setRoot(strtok(NULL, " \t\v"));
//                        std::cout << servers[i]->getRoot() << std::endl;
                    } else if (!strcmp(word, "limit_body_size") && servers[i]->getLimitBodySize() == 0) {
                        servers[i]->setLimitBodySize(atoi(strtok(NULL, " \t\v")));
//                        std::cout << servers[i]->getLimitBodySize() << std::endl;
                    } else if (!strcmp(word, "error_page")) {
                        word = strtok(NULL, " \t\v");
                        servers[i]->setErrorPage(word, strtok(NULL, " \t\v"));
//                        servers[i]->getErrorPage();
                    } else if (!strcmp(word, "}")) {
                            break;
                    } else {
                        throw "Error in configuration file.";
                    }
                    word = strtok(NULL, " \t\v");

//                            case 'location':
//                            case 'methods_allowed':
//                                if (servers[i]->getMethodsAllowed() == "")
//                                    servers[i]->setMethodsAllowed(strtok(NULL, " \t\v")); // Доделать
//                                else
//                                    throw "Error in configuration file.";
//                                break;
// как в конфиг записать?              case 'cgi':

                }
            } else
                throw "Error in configuration file.";

            word = strtok(NULL, " \t\v");
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
