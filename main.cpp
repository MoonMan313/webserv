#include "WebServer.hpp"

int main(int argc, char **argv)
{
    WebServer test;

	if (argc > 2)
	{
		std::cout << "Enter one configuration file." << std::endl;
		return 0;
	}
	ParserConfig config(argv[1]);

    test.setup();
    test.connect(config);
    test.close_fds();
    return 0;
}
