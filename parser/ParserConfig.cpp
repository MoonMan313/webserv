//
// Created by Cheryle Dionna on 05/05/2022.
//
#include <cstring>
#include <cstdlib>
#include "ParserConfig.hpp"

ParserConfig::ParserConfig(char *pathConfig) {
    const char *fileName;
    std::string line;

    if (pathConfig)
        fileName = pathConfig;
    else
        fileName = DEFAULT_CONFIG;
    std::ifstream   file(fileName);

    try{
        if (!file.is_open())
            throw "Failed to open configuration file.";
        while (std::getline(file, line)) {
            char *str = const_cast<char *>(line.c_str());
            char *word = std::strtok(str," \t\v");
            int i = -1;
            while (word != NULL) {
                if (!strcmp("server", word)) {
                    word = strtok(NULL, " \t\v");
                    if (!strcmp("{", word)){
                        servers.push_back(new Server());
                        ++i;
                        switch (word = strtok(NULL, " \t\v")){
                            case 'host':
                                if (servers[i]->getHost() == "")
                                    servers[i]->setHost(strtok(NULL, " \t\v"));
                                else
                                    throw "Error in configuration file.";
                                break;
                            case 'port':
                                if (servers[i]->getPort() == 0)
                                    servers[i]->setPort(atoi(strtok(NULL, " \t\v")));
                                else
                                    throw "Error in configuration file.";
                                break;
                            case 'server_name':
                                if (servers[i]->getServerNames() == "")
                                    servers[i]->setServerNames(strtok(NULL, " \t\v"));
                                else
                                    throw "Error in configuration file.";
                                break;
                            case 'methods_allowed':
                                if (servers[i]->getMethodsAllowed() == "")
                                    servers[i]->setMethodsAllowed(strtok(NULL, " \t\v")); // Доделать
                                else
                                    throw "Error in configuration file.";
                                break;
                            case 'autoindex':
                                if (servers[i]->isAutoindex() == 0) {
                                    word = strtok(NULL, " \t\v");
                                    if (word == "on")
                                        servers[i]->setAutoindex(1);
                                    else if (word != "off")
                                        throw "Error in configuration file.";
                                }
                                else
                                    throw "Error in configuration file.";
                                break;
                            case 'index':
                                if (servers[i]->getIndex() == "")
                                    servers[i]->setIndex(strtok(NULL, " \t\v"));
                                else
                                    throw "Error in configuration file.";
                                break;
// как делать?              case 'cgi':
//
//                                break;
                            case 'redirection':
                                if (servers[i]->getRedirection() == "")
                                    servers[i]->setRedirection(strtok(NULL, " \t\v"));
                                else
                                    throw "Error in configuration file.";
                                break;
                            case 'root':
                                if (servers[i]->getRoot() == "")
                                    servers[i]->setRoot(strtok(NULL, " \t\v"));
                                else
                                    throw "Error in configuration file.";
                                break;
                            case 'limit_body_size':
                                if (servers[i]->getLimitBodySize() == 0)
                                    servers[i]->setLimitBodySize(atoi(strtok(NULL, " \t\v")));
                                else
                                    throw "Error in configuration file.";
                                break;
                            case 'error_page':
                                word = strtok(NULL, " \t\v");
                                servers[i]->setErrorPage(word, strtok(NULL, " \t\v"));
                                break;
                            case 'location':

                                break;
                            default:
                                throw "Error in configuration file.";
                        }
                    }
                    else
                        throw "Error in configuration file.";
                }
                word = strtok(NULL, " \t\v");
            }
        }
    }
    catch (const char *text_error){
        std::cout << text_error << std::endl;
    }
    file.close();
}


//const std::vector<Server *> &ParserConfig::getServers() const {
//
//}
