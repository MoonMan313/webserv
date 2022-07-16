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

std::string get_file_data(std::string filename, Request *req)
{
	std::string	out;

	if (filename.find(".cgi", filename.length() - 4) != std::string::npos)
	{
		Cgi cgi(filename);
		cgi.executeCgi(*req);
		out = cgi.getResp();
	}
	else
		out = file_to_str(filename);
	if (out == "")
		req->setRespStatus(500); // internal error
	else
		req->setRespStatus(200);
	return (out);
};

void apply_referer(Request *req)
{
	std::string	temp_path;
	int			pos;

	pos = req->getHeaders()["Referer"].find(req->getPort());
	temp_path = req->getHeaders()["Referer"].substr(pos + \
		req->getPort().length());
	if (is_file(temp_path))
		temp_path = temp_path.substr(0, temp_path.rfind("/"));
	if (temp_path.back() == '/')
		temp_path.pop_back();
	temp_path = temp_path + req->getPath();
	std::cout << "path w referer: " << temp_path << std::endl;
	req->setPath(temp_path);
};


int check_method(std::vector<std::string> vec, std::string meth)
{
	std::vector<std::string>::iterator it;

	it = vec.begin();
	while (it != vec.end())
	{
		if (*it == meth)
			return (1);
		it++;
	};
	return (0);
}

int method_allowed(Server *serv, char const *loc_name, Request *req)
{	
	std::cout << "location METHODS: " << serv->getLocation(loc_name)->getMethodsAllowed().empty() << std::endl;
	if (!serv->getLocation(loc_name)->getMethodsAllowed().empty())
	{
		if (check_method(serv->getLocation(loc_name)->getMethodsAllowed(), req->getMethod()))
			return (1);
	}
	else 
	{	
		if (check_method(serv->getMethodsAllowed(), req->getMethod()))
			return (1);
	}	
	return (0);
}

std::string check_locations(Server *serv, std::string file_extension, std::string temp_path, Request *req)
{
	//first check if location appeared w file extension
	std::cout << "FILE EXTENSION: " << file_extension << std::endl;
	std::cout << "PATH IS: " << temp_path << serv->getLocation(temp_path.c_str()) << std::endl;
	if (serv->getLocation(file_extension.c_str()) != NULL)
	{
		//check if method is allowd
		if (method_allowed(serv, file_extension.c_str(), req))
		{
			std::cout << "We got bla file -> let's see incoming path: " << temp_path << std::endl;
			return (serv->getLocation(file_extension.c_str())->getRoot() + "cgi_bin/cgi_tester");
		}
		else
			req->setRespStatus(405);
			//err page method not allowe
	}
	else if (serv->getLocation(temp_path.c_str()) != NULL)
	{
		if (method_allowed(serv, temp_path.c_str(), req))
		{
			// define path
			std::cout << "We got path, not file -> let's see incoming path: " << temp_path << std::endl;
			return (serv->getLocation(temp_path.c_str())->getRoot());
		}
		else
			req->setRespStatus(405);
			//err page method not allowe
	}
	else
		req->setRespStatus(404);
	return ("");
};