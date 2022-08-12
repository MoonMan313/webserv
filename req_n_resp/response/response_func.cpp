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

std::string get_file_data(std::string filename, Request *req, Server *serv)
{
	std::string	out;

	if (filename.find(".cgi", filename.length() - 4) != std::string::npos)
	{
		std::cout << "CGI FILENAME" << filename << "AND SERV" << serv->getCgi() << std::endl;
		if (serv->getCurrLocation()->getCgi() == "" && \
			serv->getCgi() == "")
			out = "";
		else if (serv->getCurrLocation()->getCgi() != "" && \
			filename.find(serv->getCurrLocation()->getCgi())\
			== std::string::npos)
			out = "";
		else if (filename.find(serv->getCgi()) == std::string::npos && \
		serv->getCurrLocation()->getCgi() == "" )
			out = "";
		else
		{
			Cgi cgi(filename);
			cgi.executeCgi(*req);
			out = cgi.getResp();
		}
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

std::string define_file_for_root(std::string temp_root, \
	std::string temp_path, Server *serv, Request *req)
{
// if location is empty string -> goto respStatus and make proper err response
	if (temp_root == "")
		return ("");
	temp_root = temp_root + \
	req->getPath().substr(req->getPath().find(temp_path) + temp_path.length());
	while (temp_root.find("//") != std::string::npos)
	{
		temp_root.erase(temp_root.find("//"), 1);
	}
	if (!(is_file(temp_root)) && temp_root.find("cgi_tester") == \
		std::string::npos)
	{
		if (serv->getCurrLocation()->getIndex() != "")
			temp_root = temp_root + serv->getCurrLocation()->getIndex();
		else
			temp_root = temp_root + serv->getIndex();
	};
	return (temp_root);
}

std::string check_locations(Server *serv, std::string file_extension, \
	std::string temp_path, Request *req)
{
	//first check if location appeared w file extension
	if (serv->getLocation(file_extension.c_str()) != NULL)
	{
		//check if method is allowed
		serv->setCurrLocation(serv->getLocation(file_extension.c_str()));
		if (method_allowed(serv, file_extension.c_str(), req))
		{
			//check here, probably single file with .bla shall send post request to cgi_tester in root
			return define_file_for_root(\
				(serv->getLocation(file_extension.c_str())->getRoot() + \
				"/cgi_tester"), temp_path, serv, req);
		}
		else
			req->setRespStatus(405);
			//err page method not allowed
	}
	else if (serv->getLocation(temp_path.c_str()) != NULL)
	{
		std::cout << "here" << std::endl;
		serv->setCurrLocation(serv->getLocation(temp_path.c_str()));
		if (method_allowed(serv, temp_path.c_str(), req))
		{
			// define path
			if (serv->getCurrLocation()->getRedirection() != "")
			{
				std::cout << "here" << std::endl;
				req->setRespStatus(301);
				return serv->getCurrLocation()->getRedirection();
			}
			else
				return define_file_for_root(serv->getLocation(temp_path.c_str())->getRoot(), \
			temp_path, serv, req);
		}
		else
		{
			std::cout << "method no allowed" << std::endl;
			req->setRespStatus(405);
		}

	}
	else
	{
		req->setRespStatus(404);
	}
	return ("");
};

int check_limit(Request *req, Server *serv)
{
	if (req->getHeaders()["Content Length"] != "")
	{
		if (std::stoul(req->getHeaders()["Content Length"]) \
		> serv->getLimitBodySize())
		{
			req->setRespStatus(413);
			return (1);
		}
	}
	return (0);
};
