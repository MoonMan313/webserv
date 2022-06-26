#include <iostream>

#include "ConfigUtils.hpp"

int main(int argc, char **argv) {
    if (argc > 2){
        std::cout << "Enter one configuration file." << std::endl;
        return 0;
    }
    ParserConfig config(argv[1]);
    return 0;
}
