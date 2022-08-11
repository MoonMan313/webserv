#include "Iosck.hpp"

Iosock::Iosock(){};

Iosock::Iosock(int fd)
{
	_fd = fd;
	fcntl(_fd, F_SETFL, O_NONBLOCK);
};

//mb some remove actions here
Iosock::~Iosock(){};

std::string Iosock::get_message()
{
	std::string out;

	out = this->_msgs.front();
	std::cout << "RECEIVED MSG IS: \n\n";
	std::cout << out << std::endl << std::endl;
	this->_msgs.pop();
	return (out);
};

int Iosock::read_message()
{
	char				buffer[3000];
	std::stringstream	str;
	int				rc;

	while((rc = recv(this->_fd, buffer, sizeof(buffer) - 1, 0) > 0)
	{
		if (rc > 0)
		{
			std::cout << "pushing to _msg" << std::endl;
			buffer[rc] = '\0';
			str = buffer;
			this->_msgs.push(str);
		}
	}

	std::cout << "recv here done and rc " << rc << std::endl;
	//should  be different and in connection to fd may  be
	//req.parse_request(buffer);
	//resp.make_response(config, req);


	if (rc == 0)
	{
		std::cout << "Connection will be closed" << std::endl;
		//close_conn = TRUE;
	//	break;
	}
	return (rc);
}
