#ifndef IOSCK_HPP
# define IOSCK_HPP

#include "response.hpp"

class Iosock
{
private:
	std::string				_current_message;
	std::queue<std::string>	_msgs;
	int						_fd;
public:
	Iosock();
	Iosock(int fd);
	~Iosock();
	std::string				get_message();
	void					read_message();
	void					show();


	void 					set_sck(int fd) {this->_fd = fd;};

	int						get_sck(void) {return _fd;};
	int						empty_sock(void) {return _msgs.empty();};
};

#endif
