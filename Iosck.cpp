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

std::vector<int> get_chunk_size(int start, std::vector<char> vec)
{
	int			chunk_size;
	int			end_num_pos;
	std::string	temp;
	std::vector<int> out;

	end_num_pos = start - 1;
	while(vec.at(++end_num_pos) != '\r')
		temp.push_back(vec.at(end_num_pos));
	end_num_pos += 2;
	chunk_size = std::stol(temp, NULL, 16);
	out.push_back(end_num_pos);
	out.push_back(chunk_size);

	return (out);
};

int	Iosock::get_transfer_encoding()
{
//	int chunk_start;
//	std::string chunk_size;
	int chunk_int;
	std::vector<char>::iterator it;
	size_t pos;

	if (this->_input_vec.size() > 4)
	{
		pos = _input_vec.size() - 1;
		if (_input_vec.at(pos) == '\n' && \
			_input_vec.at(pos - 1) == '\r' && \
			_input_vec.at(pos - 2) == '\n' && \
			_input_vec.at(pos - 3) == '\r')
			{
			//find body part
			pos = 4;
			std::cout <<  "CURR IS " << _current_message << std::endl;
			while (++pos < _input_vec.size())
			{
				if(_input_vec.at(pos) == '\n')
					if (_input_vec.at(pos - 1) == '\r' && \
						_input_vec.at(pos - 2) == '\n' && \
						_input_vec.at(pos - 3) == '\r')
						{
							std::cout << "INSIDE and pos " << pos << " and cm len" << _input_vec.size() << std::endl;
							pos++;
							if (pos == _input_vec.size())
								return (0);
							break ;
						}
			}
			it = _input_vec.begin();
			while(1)
			{
				//check chunk size
				chunk_int = get_chunk_size(pos, _input_vec).at(1);
				std::cout << "CHUNK SIZE IS " << chunk_int << std::endl;

				_input_vec.erase(it + pos, it + get_chunk_size(pos, _input_vec).at(0));
				pos += chunk_int;
				//fo check
				if (_input_vec.at(pos) != '\r')
					std::cout << "MIMOMIMO" << std::endl;
				_input_vec.erase(it + pos, it + pos + 2);
				if (chunk_int == 0)
				{
					_current_message.clear();
					while(it != _input_vec.end())
					{
						_current_message.push_back(*it);
						it++;
					}
					return (1);
				}

			}
		}
	}
	return (0);
	//if (chunk_start == (int)_current_message.find("0\r\n\r\n"))
	//if (chunk_start < (int)_current_message.length())
	//{


/*
	if ( _current_message.find("\r\n\r\n", _current_message.length() - 5) == \
		std::string::npos)
		return 0;
	chunk_start = _current_message.find("\r\n\r\n") + 4;

	while (_current_message.find("\r\n", chunk_start) != std::string::npos)
	{

			chunk_size = _current_message.substr(chunk_start, \
			_current_message.find("\r\n",chunk_start) - chunk_start);
		//std::cout << "chunk size is here :" << chunk_size << std::endl;
		chunk_int = std::stol(chunk_size, NULL, 16);
		std::cout << "GO GO GO " << chunk_int << std::endl;
		if (chunk_int  == 0)
		{
			std::cout << "here me" << std::endl;
			_current_message.erase(chunk_start);
			return (1);
		}
		std::cout << "start cut at : " << chunk_start << std::endl;
		std::cout << "finish cut at : " << _current_message.find("\r\n", \
			chunk_start) + 2 - chunk_start << std::endl;
		_current_message.erase(chunk_start, _current_message.find("\r\n", \
			chunk_start) + 2 - chunk_start);
		//std::cout << "We check the end of the chunk :" << \
		//	_current_message[chunk_start + chunk_int] << std::endl;
		chunk_start += chunk_int;
		//erase end part
		_current_message.erase(chunk_start, 2);
		std::cout << "len is :" << _current_message.length() << \
			" and pos :" << chunk_start << std::endl;
	}*/
	//}
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
		if (_current_message.find("Content-Length") != std::string::npos)
		{
			temp = "Content-Length:";
			len = std::atoi(_current_message.substr\
				(_current_message.find("Content-Length") + temp.length()).c_str());
			std::cout << "have :" << (this->_input_vec.size() - _current_message.find("\r\n\r\n") - 4) \
			<< " and len is: " << len << std::endl;
			if (this->_input_vec.size() - _current_message.find("\r\n\r\n") - 4 \
			 >= len)
				return 1;
			return (0);
		}
		// check if transfer encoding is available
		if (_current_message.find("Transfer-Encoding: chunked") != std::string::npos)
		{
			if (get_transfer_encoding())
			{
				std::cout << "CHUNKED" << std::endl;
				return (1);
			}
			return (0);
		}
		std::cout << "ASSEMBLED" << "\n" << _current_message << std::endl;
		return (1);
	}
	return (0);
}

int Iosock::read_message()
{
	char				buffer[3000];
	std::stringstream	ss;
	int					rc;


	rc = recv(this->_fd, buffer, 2999, 0);
	buffer[rc] = '\0';
	ss << buffer;

	if (_current_message.empty())
		_input_vec.clear();
	int i = -1;
	while(++i < rc)
		_input_vec.push_back(buffer[i]);
//	std::cout << "buffer is ----- > " << buffer << "\n____" << std::endl;
//	rc = recv(this->_fd, buffer, sizeof(buffer) - 1, 0);
	std::cout << "rc is :" << rc << std::endl;
//	}
	std::string temp;
	temp = ss.str();
	if (temp.length() == 0)
		return (0);
	_current_message += temp;
	if (message_assembled())
	{
		std::cout << "FINAL REQ IS :\n" << _current_message << std::endl;
		this->_msgs.push(_current_message); // do if message has is ready
		//do necessary changes to curr_msg
		_current_message.clear();
	}
	if (rc == 0)
	{
		std::cout << "Connection will be closed" << std::endl;
		return (0);
	}

	return (1);
}
