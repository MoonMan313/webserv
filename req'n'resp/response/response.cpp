#include "response.hpp"

Response::Response(){};

Response::~Response(){};

//err status making
void Response::make_err_resp(int resp_status)
{
	this->setRespons("HTTP/1.1 " + std::to_string(resp_status)\
		+ " Bad Request");
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
void Response::path_assembling_n_check(std::map<std::string, \
	Location> Locations, Request *req) // here link assembled ё
{
	//check referer part to apply proper folder
	std::string temp_path;
	unsigned long slash_pos;
	if (req->getHeaders()["Referer"] != "" && \
	!is_file(req->getHeaders()["Referer"]))
		apply_referer(req);
	//check referer header in case is necessary to assemble path acc to it
	temp_path = "/";
	slash_pos = slash_or_end(req->getPath());
	if (is_file(req->getPath()))
	{
		if (slash_pos != req->getPath().length())
			temp_path = req->getPath().substr(0, slash_pos);
	}
	else
		temp_path = req->getPath().substr(0, slash_pos);
	if (Locations.count(temp_path) != 0) // check if server root should be here
	{
		this->setRoot(Locations[temp_path].root + \
			req->getPath().substr(req->getPath().find(temp_path) \
			+ temp_path.length())); // get substr with proper root naming
	}
	else
		std::cout << "failure, location not found" << std::endl;
	std::cout << this->getRoot().compare("") << " here is the num" << std::endl;
	if (!is_file(this->getRoot()) && this->getRoot().compare("") != 0)
		this->setRoot(this->getRoot() + "/" + Locations[temp_path].index);
	else if (!is_file(this->getRoot()))
		this->setRoot(Locations[temp_path].index);

	//std::cout << "root is " << this->getRoot() << std::endl;
};

void Response::execute_post(Request &req) {(void)req;};

void Response::execute_delete(Request &req) {(void)req;};

void Response::execute_get(Request &req)
{
	//(void)req;
	std::string file_data;
	if (req.getHeaders()["Referer"] == "")
		std::cout << "YES" << std::endl;
	std::cout << req.getHeaders()["Referer"] << " <- ref" << std::endl;
	std::cout << "RORUTE IS :" << this->getRoot() << "and req path is " << req.getPath() << std::endl;
	file_data = get_file_data(this->getRoot(), req);
	std::string hello = "HTTP/1.1 200 Okay\r\nContent-Transfer-Encoding: binary; Content-Length: " + \
	std::to_string(file_data.length()) + ";charset=ISO-8859-4; Accept-Language : ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7 \r\n\r\n" + file_data;
	this->setRespons(hello);
};


void Response::execute(Request &req)
{
	std::string methods[3];
	int i = -1;
	void (Response::*f[3])(Request &req) = {
		&Response::execute_get,
		&Response::execute_post,
		&Response::execute_delete,
		// method not allowd??
	};
	methods[0] = "GET";
	methods[1] = "POST";
	methods[2] = "DELETE";
	while(req.getMethod() != methods[++i]);
	(this->*f[i])(req);
};

void Response::make_response(std::map<std::string, Location> Locations, \
	Request &req)
{
	//check if status is not OK
	if (req.getRespStatus() != 200)
		return make_err_resp(req.getRespStatus());
	else
		//define _root
		path_assembling_n_check(Locations, &req);

	//execute method
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
