#include "response.hpp"

int is_file(std::string filename)
{
	if (filename.find(".") != std::string::npos)
		return (1);
	return (0);
};

std::string file_to_str(std::string filename)
{
	std::ifstream is(filename, std::ifstream::binary);
	std::string out;
	std::stringstream ss;

	if (!is.good())
		return ("");
	char *buffer;
	int length ;
	// get length of file:
	is.seekg (0, is.end);
	length = is.tellg();
	is.seekg (0, is.beg);
	buffer = new char[length];
	// read data as a block:
	is.read (buffer, length);
	ss.write(buffer, length);
	out = ss.str();
	is.close();
	// ...buffer contains the entire file...
	delete[] buffer;
	return(out);
};

std::string get_file_data(std::string filename, Request &req)
{
	std::string out;
	//std::ifstream is(filename, std::ifstream::binary);
	//std::string out;
	//std::stringstream ss;
// check if cgi and launch it
	if (filename.find(".cgi", filename.length() - 4) != std::string::npos)
	{
		Cgi cgi(filename);
		cgi.executeCgi(req);
		out = cgi.getResp();
		std::cout << "resp is:" << out << std::endl;
	}
	else
		out = file_to_str(filename);
	// check if failed to open program -> out is empty EG
	return (out);
};

void apply_referer(Request *req)
{
	std::string	temp_path;
	int			pos;

	pos = req->getHeaders()["Referer"].find(req->getPort());
	temp_path = req->getHeaders()["Referer"].substr(pos + \
		req->getPort().length());
	if (temp_path.back() == '/')
		temp_path.pop_back();
	temp_path = temp_path + req->getPath();
	req->setPath(temp_path);
};
