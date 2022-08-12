#include "Iosck.hpp"

Iosock::Iosock(){};

Iosock::Iosock(int fd)
{
	//int					on = 1;

	_fd = fd;
	fcntl(_fd, F_SETFL, O_NONBLOCK);
	//setsockopt(_fd, SOL_SOCKET,  SO_KEEPALIVE , \
	//	(char *)&on, sizeof(on));

};

//mb some remove actions here
Iosock::~Iosock(){};

std::string Iosock::get_message()
{
	std::string out;

	out = this->_msgs.front();
	//std::cout << "RECEIVED MSG IS: \n\n";
	//std::cout << out << std::endl << std::endl;
	this->_msgs.pop();
	return (out);
};

int	Iosock::get_transfer_encoding()
{
	int chunk_start;
	int chunk_size_int;
	std::string chunk_size;

	chunk_start = _current_message.find("\r\n\r\n") + 4;
	if (chunk_start < (int)_current_message.length())
	{
		if (_current_message.find("\r\n", chunk_start) != std::string::npos)
		{
			chunk_size = _current_message.substr(chunk_start, \
				_current_message.find("\r\n",chunk_start));
			chunk_size_int = std::stol(chunk_size, NULL, 16);
			_current_message.erase(chunk_start, _current_message.find("\r\n", \
			chunk_start) + 2 - chunk_start);
			std::cout << "We check the end of the chunk :" << _current_message[chunk_start + chunk_size_int] << std::endl;
		}
		//check if all chuncks assembled
		if (_current_message.find("0\r\n\r\n"))
		{
			_current_message.erase(_current_message.find("0\r\n\r\n"),5);
			return (1);
		}
	}

	return (0);
}

int	Iosock::message_assembled()
{
	size_t len;
	std::string temp;
	//check if end of the message body is not yet available
	if (_current_message.find("\r\n\r\n") != std::string::npos)
	{
		//check if content length is available
		std::cout << "INSIDER " << _current_message.find("\r\n\r\n") << std::endl;
		if (_current_message.find("Content-Length") != std::string::npos)
		{
			temp = "Content-Length:";
			len = std::atoi(_current_message.substr\
				(_current_message.find("Content-Length") + temp.length()).c_str());
			temp = \
			_current_message.substr(_current_message.find("\r\n\r\n") + 4);
			if (temp.length() >= len)
				return 1;
		}
		// check if transfer encoding is available
		if (_current_message.find("Transfer-Encoding: chunked") != std::string::npos)
		{
			if (get_transfer_encoding())
				return (1);
			return (0);
		}
		return (1);
	}
	return (0);
}

int Iosock::read_message()
{
	char				buffer[3000];
	std::stringstream	ss;
	//std::string			str_out;
	int				rc;
	rc = recv(this->_fd, buffer, sizeof(buffer) - 1, 0);
	while(rc > 0)
	{
		buffer[rc] = '\0';
		ss << buffer;
		std::cout << "buffer is ----- > " << buffer << "\n____" << std::endl;
		rc = recv(this->_fd, buffer, sizeof(buffer) - 1, 0);
		std::cout << "rc is :" << rc << std::endl;
	}
	std::string temp;
	temp = ss.str();
	if (temp.length() == 0)
		return (0);
	_current_message += temp;
	if (message_assembled())
	{
		std::cout << "me insife" << std::endl;
		this->_msgs.push(_current_message); // do if message has is ready
		//do necessary changes to curr_msg
		_current_message = "";
	}
	if (rc == 0)
	{
		std::cout << "Connection will be closed" << std::endl;
		return (0);
	}

	return (1);
}
