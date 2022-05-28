#include "Server.hpp"

int main()
{
    Server test;

    test.setup();
    test.connect();
    test.close_fds();
    return 0;
}