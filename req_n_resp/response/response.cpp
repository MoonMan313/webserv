#include "response.hpp"

Response::Response(){};

Response::~Response(){};

//err status making
void Response::make_err_resp(int resp_status)
{
	this->setRespons("HTTP/1.1 " + std::to_string(resp_status)\
		+ "Bad Request");
		// add link to error page from CONFIG (abs patg to err file)
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
	std::string temp_root;

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
	temp_root = check_locations(serv, file_extension, temp_path, req);

	std::cout << "root before marriage: " << temp_root << " and temp path" << temp_path << " and req path: " << req->getPath() << std::endl;
	this->setRoot("");
	// if location is empty string -> goto respStatus and make proper err response
	if (temp_root == "")
		return ;
	temp_root = temp_root + req->getPath().substr(req->getPath().find(temp_path) + temp_path.length());
	while (temp_root.find("//") != std::string::npos)
	{
		temp_root.erase(temp_root.find("//"), 1);
	}
	if (!(is_file(temp_root)))
	{
		if (serv->getLocation)
	};
	
	std::cout << "temp root to file is " << temp_root << std::endl;

	this->setRoot("");
	/*if (Locations.count(temp_path) != 0) // check if server root should be here
	{
        std::cout << Locations[temp_path].root << "<- LOCATION ROOT" << std::endl;
        this->setRoot(Locations[temp_path].root + \
			req->getPath().substr(req->getPath().find(temp_path) \
			+ temp_path.length())); // get substr with proper root naming
        std::cout << this->getRoot() << "<- ROOT" << std::endl;
	}
	else
		std::cout << "failure, location not found"  << std::endl;
	std::cout << this->getRoot().compare("") << " here is the num" << Locations[temp_path].index << std::endl;
	if (!is_file(this->getRoot()) && this->getRoot().compare("") != 0)
		this->setRoot(this->getRoot() + "/" + Locations[temp_path].index);
	else if (!is_file(this->getRoot()))
		this->setRoot(Locations[temp_path].index);*/
};

void Response::execute_post(Request req)
{
	//status 202 - Accepted status to send in response in case file is accepted

	(void)req;

};

void Response::execute_delete(Request req)
{
	//check status with girls video
	(void)req;

};

int	check_file(std::string filename)
{
	FILE	*pFile;
	int		out = 1;

	pFile = fopen(filename.c_str(), "r");
	if (pFile == NULL)
		out = -1;
	fclose (pFile);
	return (out);
};

void Response::execute_get(Request req)
{
	//think here how to implement redirect
	std::string file_data;
	std::string hello;

	if (check_file(this->getRoot()) != -1)
		file_data = get_file_data(this->getRoot(), &req);
	//check here if file-data is not awailable;
	if (req.getRespStatus() != 200)
		make_err_resp(req.getRespStatus());
	else
	{
		//hello = "HTTP/1.1 301 Okay\r\nLocation : https://yandex.ru/";
		//;\
		// Accept-Language : " + req.getHeaders()["Accept-Language"] \
		//+ " \r\n\r\n";

		hello = "HTTP/1.1 200 Okay\r\nContent-Transfer-Encoding: binary; \
		Content-Length: " + std::to_string(file_data.length()) + \
		"; Accept-Language : " + req.getHeaders()["Accept-Language"] \
		+ " \r\n\r\n" + file_data;
		this->setRespons(hello);
	}
};

void Response::execute(Request req)
{
	std::string methods[3];
	int i = -1;
	void (Response::*f[3])(Request req) = {
		&Response::execute_get,
		&Response::execute_post,
		&Response::execute_delete,
	};
	methods[0] = "GET";
	methods[1] = "POST";
	methods[2] = "DELETE";
	while(req.getMethod() != methods[++i]);
	(this->*f[i])(req);
};

Server *Response::server_availabe(ParserConfig config, Request req)
{
	std::vector<Server *>::const_iterator  it;

	it = config.getServers().begin();
	while (it != config.getServers().end())
	{
		if ((*it)->getPort() == std::stoul(req.getPort()) && (*it)->getHost() == req.getHost())
			return (*it);
		it++;
	}
	return NULL;
}

void Response::make_response(ParserConfig config, Request req)
{
	Server *serv;
	if (req.getRespStatus() != 200)
		return make_err_resp(req.getRespStatus());
	serv = server_availabe(config, req);
	if (!serv)
		return make_err_resp(404);
	//check if server is availble?
	path_assembling_n_check(serv, &req);
	//check if req failures arrived
	if (req.getRespStatus() != 200)
		return make_err_resp(req.getRespStatus());
	execute(req);
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
