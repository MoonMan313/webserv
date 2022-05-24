//
// Created by Cheryle Dionna on 05/05/2022.
//

#include "ParserConfig.hpp"

ParserConfig::ParserConfig(char *pathConfig) {
    const char *fileName;
    std::string line;

    if (pathConfig)
        fileName = pathConfig;
    else
        fileName = DEFAULT_CONFIG;
    std::cout << fileName << std::endl;
    std::ifstream   file(fileName);
    if (!file.is_open())
        throw "Failed to open configuration file.";
    while (!file.eof()) {
        file >> line;
        if (line == "server"){

        }
        while (std::getline(file, line)) {
            line.
                    std::cout << line << std::endl;
        }
    }
    file.close();
}

//const std::vector<Server *> &ParserConfig::getServers() const {
//
//}
