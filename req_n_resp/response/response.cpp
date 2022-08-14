#include "response.hpp"

Response::Response(){setErrPages();};

Response::~Response(){};

void Response::setErrPages(void)
{
	_err_pages[200] = "Okay";
	_err_pages[202] = "Accepted";
	_err_pages[206] = "Partial Content";
	_err_pages[301] = "Moved Permanently";
	_err_pages[302] = "Found";
	_err_pages[400] = "Bad Request";
	_err_pages[404] = "Not Found";
	_err_pages[405] = "Method Not Allowed";
	_err_pages[413] = "Request Entity Too Long";
	_err_pages[500] = "Internal Server Error";
	_err_pages[505] = "HTTP Version Not Supported";
};

//err status making
void Response::make_err_resp(Server *serv, Request req)
{
	//may be this type of logic make no sense due to failure header status
	std::string err_response;
	std::string hello;
	(void)serv;

	err_response = "ERROR STATUS : " + std::to_string(req.getRespStatus()) + \
		"\nTHIS IS: " + _err_pages[req.getRespStatus()] + " FAILURE. \nCIAO";
	hello = "HTTP/1.1 " + std::to_string(req.getRespStatus()) + " " + \
		_err_pages[req.getRespStatus()] + "\r\nContent-Length: " +  \
		std::to_string(err_response.length()) + \
		+ " \r\n\r\n" + err_response;
	if (req.getMethod() == "HEAD")
		hello = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
	this->setRespons(hello);
};

unsigned long slash_or_end(std::string str)
{
	unsigned long slash_or_end;
	//check slash after first once
	slash_or_end = str.length();
	if (str.substr(1, str.length() - 1).find("/") != std::string::npos)
		slash_or_end = str.substr(1, str.length() - 1).find("/") + 1;
	return (slash_or_end);
}

// make _resp sting to send to request
void Response::path_assembling_n_check(Server *serv, Request *req)
{
	//check referer part to apply proper folder
	std::string temp_path;
	unsigned long slash_pos;
	std::string	file_extension;

	if (is_file(req->getPath()))
		file_extension = req->getPath().substr(req->getPath().rfind("."));
	//check referer header in case is necessary to assemble path acc to it
	if (req->getHeaders()["Referer"] != "")	// \ && !is_file(req->getHeaders()["Referer"]))
		apply_referer(req);
	//std::cout << "path is " << req->getPath() << "adn " << req->getHeaders()["Referer"]  << std::endl;
	temp_path = "/";
	slash_pos = slash_or_end(req->getPath());
	std::cout << "slash: " << slash_pos << std::endl;
	if (is_file(req->getPath()))
	{
		if (slash_pos != req->getPath().length())
			temp_path = req->getPath().substr(0, slash_pos);
	}
	else
		temp_path = req->getPath().substr(0, slash_pos);
	//here temp path should be compared w server ROOT field where file extension or folder name shall be searched
	std::cout << "temp path" << temp_path << std::endl;
	temp_path = check_locations(serv, file_extension, temp_path, req);
	// redirection happenns here
	std::cout << "temp path after" << temp_path << std::endl;
	this->setRoot(temp_path);
};

int	check_file(std::string filename)
{
	FILE	*pFile;
	int		out = 1;

	pFile = fopen(filename.c_str(), "r");
	if (pFile == NULL)
	{
		out = -1;
	}
	fclose (pFile);
	return (out);
};

void Response::execute_put(Request req, Server *serv)
{
	(void)serv;
	std::string hello;
	std::string reply;

	std::cout << "I TYR TO PUT" << std::endl;
	std::ofstream file(this->getRoot(), std::ofstream::binary);
	if (!req.getHeaders()["Content-Length"].empty() || \
		!req.getHeaders()["Transfer-Encoding"].empty())
		file.write(req.getBodyPut().data(), req.getBodyPut().size());
	file.close();
	std::cout << "DONE" << std::endl;
	reply  = "we got you file baby";
	hello = "HTTP/1.1 201 Created\r\nContent-Length: " +  \
	std::to_string(reply.length()) + \
	+ " \r\n\r\n" + reply;
	this->setRespons(hello);
};


void Response::execute_delete(Request req, Server *serv)
{
	//check status with girls video
	if (check_file(this->getRoot()) != -1)
	{
		std::remove(this->getRoot().c_str());
		this->setRespons("HTTP/1.1 200 Okay\r\n\r\n");
	}
	else
	{
		req.setRespStatus(404);
		make_err_resp(serv, req);
	}
};

void Response::execute_get(Request req, Server *serv)
{
	//think here how to implement redirect
	std::string file_data;
	std::string hello;

	//check here if file-data is not awailable;
	if (serv->getCurrLocation()->getRedirection() != "")
	{
		hello = "HTTP/1.1 301 Moved Permanently\r\nLocation : " + \
		serv->getCurrLocation()->getRedirection() + "\r\n\r\n";

		this->setRespons(hello);
	}
	else
	{
		//check for redirect here mayt be
		if (check_file(this->getRoot()) != -1)
		{
			file_data = get_file_data(this->getRoot(), &req, serv);
			hello = "HTTP/1.1 200 Ok\r\nContent-Length: " + std::to_string(file_data.length()) + \
			+ " \r\n\r\n" + file_data;
			this->setRespons(hello);
		}
		else
		{
			req.setRespStatus(404);
			make_err_resp(serv, req);
		}
	}
};

void Response::execute(Request req, Server *serv)
{
	std::string methods[4];
	int i = -1;
	void (Response::*f[4])(Request req, Server *serv) = {
		&Response::execute_get,
		&Response::execute_get,
		&Response::execute_delete,
		&Response::execute_put
	};
	methods[0] = "GET";
	methods[1] = "POST";
	methods[2] = "DELETE";
	methods[3] = "PUT";
	while(req.getMethod() != methods[++i]);
	(this->*f[i])(req, serv);
};

Server *Response::server_availabe(ParserConfig config, Request req)
{
	std::vector<Server *> servers;
	std::vector<Server *>::const_iterator it;

	std::cout << "I TRY TO GET PORT " << std::stoul(req.getPort()) << std::endl;
	servers = config.getServers();
	it = servers.begin();
	while (it != servers.end())
	{
		if ((*it)->getPort() == std::stoul(req.getPort()) && \
		(*it)->getHost() == req.getHost())
		{
			return (*it);
		}
		it++;
	}
	return NULL;
}

void Response::make_response(ParserConfig config, Request req)
{
	Server *serv;
	serv = server_availabe(config, req);
	if (req.getRespStatus() != 200)
		return make_err_resp(serv, req);
	if (check_limit(&req, serv))
		return make_err_resp(serv, req);
	if (!serv) // where to send?
	{
		req.setRespStatus(404);
		return make_err_resp(serv, req);
	}
	//check if server is availble?
	path_assembling_n_check(serv, &req);
	std::cout << "path is " << req.getPath() << std::endl;
	//check if req failures arrived
	if (std::to_string(req.getRespStatus())[0] != '2' && \
	std::to_string(req.getRespStatus())[0] != '3')
		return make_err_resp(serv, req);
	execute(req, serv);
}

//SETTERS
void Response::setRoot(std::string root)
{
	this->_root = root;
}

void Response::setRespons(std::string resp)
{
	this->_response = resp;
}

//GETTTERS
std::string Response::getRoot()
{
	return(this->_root);
}

std::string Response::getRespons()
{
	return (this->_response);
}
