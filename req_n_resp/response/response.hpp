#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ParserConfig.hpp"

#define S_200 "OK"

# define S_400 "Bad Request"
# define S_405 "405 Method Not Allowed"

# define S_505 "HTTP Version Not Supported"

class Response
{
private:
	std::string _response;
	std::string _root;

public:
	Response(); // server + request
	~Response();

	// check request with config
	//void port_check(); // here link to server location Server class here

	void make_response(ParserConfig config, Request req);

	void path_assembling_n_check(Server *serv, Request *req); // here link assembled ё
	//	with root locatiion from Config
	// open question - if is a file of diff type
	// open resourse which is not present on a server?

	void execute(Request req);
	void execute_get(Request req);
	void execute_post(Request req);
	void execute_delete(Request req);

	void make_err_resp(int resp_status);
	Server *server_availabe(ParserConfig config, Request req);

	//setters
	void setRoot(std::string root);
	void setRespons(std::string resp);
	//getters
	std::string getRoot();
	std::string getRespons();
};

std::string get_file_data(std::string filename, Request *req);
int is_file(std::string filename);
void apply_referer(Request *);
std::string check_locations(Server *serv, std::string file_extension, std::string temp_path, Request *req);

#endif
