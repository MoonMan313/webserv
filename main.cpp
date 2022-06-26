#include "WebServer.hpp"

int main()
{
    Webserver test;

    test.setup();
    test.connect();
    test.close_fds();
    return 0;
}