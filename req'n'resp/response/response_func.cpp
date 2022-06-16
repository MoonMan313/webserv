#include "response.hpp"

int is_file(std::string filename)
{
	if (filename.find(".") != std::string::npos)
		return (1);
	return (0);
};

std::string get_file_data(std::string filename)
{
//	std::string out;
	std::ifstream is (filename, std::ifstream::binary);
	std::stringstream ss;
	std::string out;
	if (is) {
		// get length of file:
		is.seekg (0, is.end);
		int length = is.tellg();
		is.seekg (0, is.beg);
		char *buffer = new char[length];
		// read data as a block:
		is.read (buffer,length);
		ss.write(buffer, length);
		out = ss.str();
		is.close();
		// ...buffer contains the entire file...
		delete[] buffer;
	}
	return (out);
};
