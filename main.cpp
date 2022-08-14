#include "Iosck.hpp"
#define TRUE             1
#define FALSE            0
#define SERVER_PORT 8070


int launch_listen_sd(int port)
{
	int    				rc;
	int   				listen_sct = -1;
	struct sockaddr_in	addr;
	int					on = 1;

	listen_sct = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sct < 0)
	{
		perror("socket() failed");
		exit(-1);
	}
	rc = setsockopt(listen_sct, SOL_SOCKET,  SO_REUSEADDR, \
		(char *)&on, sizeof(on));
	if (rc < 0)
	{
		perror("setsockopt() failed");
		close(listen_sct);
		exit(-1);
	}
	int listen_new = fcntl(listen_sct, F_SETFL, O_NONBLOCK);
	if (listen_new < 0)
	{
		std::cerr << "FCNTL doesn't work" << std::endl;
		return (-1);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	rc = bind(listen_sct, (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0)
	{
		perror("bind() failed");
		close(listen_sct);
		exit(-1);
	}
	rc = listen(listen_sct, 32);
	if (rc < 0)
	{
		perror("listen() failed");
		close(listen_sct);
		exit(-1);
	}
	return (listen_sct);
}

int vec_search(std::vector<int> sockets, int val)
{
	std::vector<int>::iterator it;

	it = sockets.begin();
	while(it != sockets.end())
	{
		if ((*it) == val)
		{
			std::cout << "FOUND in vecs " << val << std::endl;
			return (1);
		}
		it++;
	};

	return 0;
}

std::string make_response_2(Iosock *io_ss, ParserConfig config)
{
	Request req;
	Response resp;

	std::string str = io_ss->get_message();

	req.parse_request(str, io_ss->get_vector());
	std::cout << "STATUS is " << req.getRespStatus() << std::endl;
	resp.make_response(config, req);
	//std::cout << "get resp" << resp.getRespons() << std::endl;
	return (resp.getRespons());
};

std::vector<int> get_listen_ss(ParserConfig config)
{
	std::vector<int> out;
	std::vector<Server *> servers;
	std::vector<Server *>::const_iterator it;

	servers = config.getServers();
	it = servers.begin();
	while (it != servers.end())
	{
		out.push_back(launch_listen_sd((*it)->getPort()));
		it++;
	}
	return out;
}

int accept_sckt(int fd)
{
	struct sockaddr_in	empty_addr;
	int					addr_s;
	int					new_sct;

	addr_s = sizeof(addr_s);
	new_sct = accept(fd, (struct sockaddr *)&empty_addr, (socklen_t*)&addr_s);
	if (new_sct == -1)
	{
		perror("accept issue");
		close(fd);
		exit(EXIT_FAILURE);
	}
	std::cout << "  New incoming connection - " << std::to_string(new_sct) << std::endl;
	return new_sct;
}

int main (int argc, char **argv)
{
	int				new_sd = -1;
	int				compress_array = FALSE;
	int				close_conn;
	int				timeout;
	struct pollfd	fds[200];
	int    			nfds = 0, current_size = 0, i, j;
	std::vector<int> listen_ss;
	std::map<int, Iosock *> io_ss;
	std::string resp;

	if (argc > 2)
	{
		std::cout << "Enter one configuration file." << std::endl;
		return 0;
	}
	ParserConfig config(argv[1]);

	listen_ss = get_listen_ss(config);
	memset(fds, 0 , sizeof(fds));
	size_t k = 0;
	while(k < listen_ss.size())
	{
		fds[k].fd = listen_ss[k];
		fds[k].events = POLLIN;
		nfds++;
		k++;
	}
	std::cout << "NFDS is: " << nfds << std::endl;
	timeout = (3 * 60 * 1000);
	std::cout << "Waiting on poll().." << std::endl;
	signal(SIGPIPE, SIG_IGN);
	while (1)
	{
		int ret;
		ret = poll(fds, nfds, timeout);
		if (ret > 0)
		{
			std::cout << "i got from poll().." << std::endl;
			current_size = nfds;
			for (i = 0; i < current_size; i++)
			{
				if(fds[i].revents == 0)
					continue;
				if (fds[i].revents & POLLOUT)
				{
					int rc;

					rc = send(fds[i].fd, resp.c_str(), resp.length(), 0);
					std::cout << "sending rc bytes: " << rc << "and len is" << resp.length() << std::endl;
					if (rc < (int)resp.length())
						break;
					fds[i].events = POLLIN;
					if (rc < 0)
					{
						std::cout << "RC IS MINUS" << std::endl;
						perror("");
						close_conn = TRUE;
						break;
					}
				}
				else if (fds[i].revents & POLLIN)
				{
					if (vec_search(listen_ss, fds[i].fd))
					{
						std::cout << "Listening socket is readable from " << fds[i].fd << std::endl;
						new_sd = accept_sckt(fds[i].fd);
						io_ss[new_sd] = new Iosock(new_sd);
						fds[nfds].fd = new_sd;
						fds[nfds].events = POLLIN;
						fds[nfds].revents = 0;
						nfds++;
						break;
					}
					else
					{
				//		std::cout << "Descriptor "<< std::to_string(fds[i].fd) << " is readable" << std::endl;
						close_conn = FALSE;
						if (io_ss[fds[i].fd]->read_message() <= 0)
							close_conn = TRUE;
						if (io_ss[fds[i].fd]->empty_sock() == 0 && \
							close_conn == FALSE)
						{
							resp = make_response_2(io_ss[fds[i].fd], \
								config);
							fds[i].events = POLLOUT;
							fds[i].revents = POLLOUT;
							break;
						}
						//fds[i].revents = 0; //if send is not over repeat send
						if (close_conn)
						{
							std::cout << "ceaning proc started on " << i << "and fd: " << fds[i].fd << std::endl;
							close(fds[i].fd);
							fds[i].fd = -1;
							compress_array = TRUE;
						}
						//delete buffer;
					}
				}
				fds[i].revents = 0;
				  /* End of existing connection is readable             */
			} /* End of loop through pollable descriptors              */

			/***********************************************************/
			/* If the compress_array flag was turned on, we need       */
			/* to squeeze together the array and decrement the number  */
			/* of file descriptors. We do not need to move back the    */
			/* events and revents fields because the events will always*/
			/* be POLLIN in this case, and revents is output.          */
			/***********************************************************/
			if (compress_array)
			{
				compress_array = FALSE;
				for (i = 0; i < nfds; i++)
				{
					if (fds[i].fd == -1)
					{
						for(j = i; j < nfds; j++)
						{
							fds[j].fd = fds[j+1].fd;
						}
						i--;
						nfds--;
					}
				}
			}
		}
	}
	// while (end_server == FALSE); /* End of serving running.    */

	/*************************************************************/
	/* Clean up all of the sockets that are open                 */
	/*************************************************************/
	for (i = 0; i < nfds; i++)
	{
		if(fds[i].fd >= 0)
			close(fds[i].fd);
	}
}
