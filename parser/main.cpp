#include <iostream>

#include "ConfigUtils.hpp"

int main(int argc, char **argv) {
    if (argc > 2){
        std::cout << "Enter one configuration file." << std::endl;
        return 0;
    }
    try{
        ParserConfig config(argv[1]);

    }
    catch (const char *text_error){
        std::cout << text_error << std::endl;
    }
    return 0;
}
