#include "response.hpp"

Response::Response(){};

Response::~Response(){};

//err status making
void Response::make_err_resp(Server *serv, Request req)
{
	//may be this type of logic make no sense due to failure header status
	std::string hello;
	std::string status;

	// check if root is proper now or what?
	// get root for err page
	std::cout << "CHECK RESP STATUS " << req.getRespStatus() << "and name: " << serv->getErrorPage(req.getRespStatus()) << std::endl;
	hello = "HTTP/1.1 " + std::to_string(req.getRespStatus()) + " " + \
	serv->getErrorPage(req.getRespStatus()) + "\r\n";
	/*Content-Transfer-Encoding: binary; \
	Content-Length: " + std::to_string(file_data.length()) + \
	"; Accept-Language : " + req.getHeaders()["Accept-Language"] \
	+ " \r\n\r\n" + file_data;*/
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
	temp_path = check_locations(serv, file_extension, temp_path, req);
	std::cout << "temp root to file is :" << temp_path << "<-" << std::endl;
	// redirection happenns here
	this->setRoot(temp_path);
};

void Response::execute_post(Request req, Server *serv)
{
	//status 202 - Accepted status to send in response in case file is accepted

	(void)req;
	(void)serv;

};

void Response::execute_delete(Request req, Server *serv)
{
	//check status with girls video
	(void)req;
	(void)serv;

};

int	check_file(std::string filename)
{
	FILE	*pFile;
	int		out = 1;

	pFile = fopen(filename.c_str(), "r");
	if (pFile == NULL)
	{
		out = -1;
		std::cout << "getting filedata" << std::endl;
	}
	fclose (pFile);
	return (out);
};

void Response::execute_get(Request req, Server *serv)
{
	//think here how to implement redirect
	std::string file_data;
	std::string hello;

	//check here if file-data is not awailable;
	if (serv->getCurrLocation()->getRedirection() != "")
	{
		std::cout << "HERE" << std::endl;
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
		}


		hello = "HTTP/1.1 200 Okay\r\nContent-Transfer-Encoding: binary; \
		Content-Length: " + std::to_string(file_data.length()) + \
		"; Accept-Language : " + req.getHeaders()["Accept-Language"] \
		+ " \r\n\r\n" + file_data;
		this->setRespons(hello);
	}
};

void Response::execute(Request req, Server *serv)
{
	std::string methods[3];
	int i = -1;
	void (Response::*f[3])(Request req, Server *serv) = {
		&Response::execute_get,
		&Response::execute_post,
		&Response::execute_delete,
	};
	methods[0] = "GET";
	methods[1] = "POST";
	methods[2] = "DELETE";
	while(req.getMethod() != methods[++i]);
	(this->*f[i])(req, serv);
};

Server *Response::server_availabe(ParserConfig config, Request req)
{
	std::vector<Server *> servers;
	std::vector<Server *>::const_iterator it;

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
	if (!serv)
	{
		req.setRespStatus(404);
		return make_err_resp(serv, req);
	}
	//check if server is availble?
	path_assembling_n_check(serv, &req);
	//check if req failures arrived
	if (req.getRespStatus() != 200 && req.getRespStatus() != 301)
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
