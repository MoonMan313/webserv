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
void Response::path_assembling_n_check(std::map<std::string, \
	Location> Locations, Request *req)
{
	//check referer part to apply proper folder
	std::string temp_path;
	unsigned long slash_pos;

	//check referer header in case is necessary to assemble path acc to it
	if (req->getHeaders()["Referer"] != "" && \
	!is_file(req->getHeaders()["Referer"]))
		apply_referer(req);
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
		hello = "HTTP/1.1 301 Okay\r\nLocation : https://yandex.ru/";
		//;\
		// Accept-Language : " + req.getHeaders()["Accept-Language"] \
		//+ " \r\n\r\n";

		/*hello = "HTTP/1.1 200 Okay\r\nContent-Transfer-Encoding: binary; \
		Content-Length: " + std::to_string(file_data.length()) + \
		"; Accept-Language : " + req.getHeaders()["Accept-Language"] \
		+ " \r\n\r\n" + file_data;*/
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

void Response::make_response(std::map<std::string, Location> Locations, \
	Request req)
{
	if (req.getRespStatus() != 200)
		return make_err_resp(req.getRespStatus());
	else
		path_assembling_n_check(Locations, &req);
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
