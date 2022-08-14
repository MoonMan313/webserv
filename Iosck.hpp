#ifndef IOSCK_HPP
# define IOSCK_HPP

#include "response.hpp"

class Iosock
{
private:
	std::string				_current_message;
	std::queue<std::string>	_msgs;
	int						_fd;
	std::vector<char> 		_input_vec;

public:
	Iosock();
	Iosock(int fd);
	~Iosock();
	std::string				get_message();
	int						read_message();
	int						message_assembled();
	int						get_transfer_encoding();
	void					show();


	void 					set_sck(int fd) {this->_fd = fd;};

	int						get_sck(void) {return _fd;};
	std::vector<char>		get_vector(void){return _input_vec;};
	int						empty_sock(void) {return _msgs.empty();};
};

#endif
