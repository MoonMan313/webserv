#include "response.hpp"

Response::Response(){};

Response::~Response(){};

// make _resp sting to send to request
void Response::path_assembling_n_check(std::map<std::string, \
	Location> Locations, Request &req) // here link assembled ё
{
	//get first folder name from req.path

	std::string temp_path;
	if (req.getPath().substr(1, req.getPath().length() - 1).find("/") \
	!= std::string::npos)
		temp_path = req.getPath().substr(1, req.getPath().substr(1, \
			req.getPath().length() - 1).find("/"));
	if (Locations.count(temp_path) != 0) // check if server root should be here
		this->setRoot(Locations[temp_path].root + \
			req.getPath().substr(1 + req.getPath().substr(1, req.getPath().length() \
			- 1).find("/"))); // get substr with proper root naming
	this->setRoot(req.getPath().substr(1)); //THIS SHOULD BE CHANGED
	std::cout << "root is " << this->getRoot() << std::endl;
	//this->setRoot(path_root);

	//next -> define file type on the end of the root and choose file or index
};

void Response::execute_post(Request &req) {(void)req;};

void Response::execute_delete(Request &req) {(void)req;};

void Response::execute_get(Request &req)
{
	(void)req;
	// define file type and make -> so far we take simply jpg
	std::string file_data;
	std::cout << this->getRoot() << std::endl;

	file_data = get_file_data(this->getRoot());

	std::string hello = "HTTP/1.1 200 Okay\r\nContent-Type: \
	image/png; Content-Transfer-Encoding: binary; Content-Length: " + \
	std::to_string(file_data.length()) + ";charset=ISO-8859-4 \r\n\r\n" + file_data;
	this->setRespons(hello);
};


void Response::execute(Request &req)
{
	std::string methods[3];
	void (Response::*f[3])(Request &req) = {
		&Response::execute_get,
		&Response::execute_post,
		&Response::execute_delete,
		// method not allowd??
	};
	methods[0] = "GET";
	methods[1] = "POST";
	methods[2] = "DELETE";
	int i = -1;
	while(req.getMethod() != methods[++i]);
	(this->*f[i])(req);
};

void Response::make_response(std::map<std::string, Location> Locations, \
	Request &req)
{
	//define _root
	path_assembling_n_check(Locations, req);

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
