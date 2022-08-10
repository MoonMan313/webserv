#include "response.hpp"
#define TRUE             1
#define FALSE            0
#define SERVER_PORT 8070

int main (int argc, char **argv)
{
	Request req;
	Response resp;
	int    len, rc, on = 1;
	int    listen_sd = -1, new_sd = -1;
	int    desc_ready, end_server = FALSE, compress_array = FALSE;
	int    close_conn;
	char   buffer[30000];
	struct sockaddr_in6   addr;
	int    timeout;
	struct pollfd fds[200];
	int    nfds = 1, current_size = 0, i, j;

	(void)desc_ready;
	(void)end_server;
	if (argc > 2)
	{
		std::cout << "Enter one configuration file." << std::endl;
		return 0;
	}
	ParserConfig config(argv[1]);

	listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
	if (listen_sd < 0)
	{
		perror("socket() failed");
		exit(-1);
	}
	(void)on;
/*rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
if (rc < 0)
{
    perror("setsockopt() failed");
    close(listen_sd);
    exit(-1);
}*/
  /*rc = ioctl(listen_sd, FIONBIO, (char *)&on);
  if (rc < 0)
  {
    perror("ioctl() failed");
    close(listen_sd);
    exit(-1);
}*/
int listen_new = fcntl(listen_sd, F_SETFL, O_NONBLOCK);
if (listen_new < 0)
{
   std::cerr << "FCNTL doesn't work" << std::endl;
   return (-1);
}
  memset(&addr, 0, sizeof(addr));
  addr.sin6_family      = AF_INET6;
  memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
  addr.sin6_port        = htons(SERVER_PORT);
  rc = bind(listen_sd, (struct sockaddr *)&addr, sizeof(addr));
  if (rc < 0)
  {
    perror("bind() failed");
    close(listen_sd);
    exit(-1);
  }
  rc = listen(listen_sd, 32);
  if (rc < 0)
  {
    perror("listen() failed");
    close(listen_sd);
    exit(-1);
  }
  memset(fds, 0 , sizeof(fds));
  fds[0].fd = listen_sd;
  fds[0].events = POLLIN;
  timeout = (3 * 60 * 1000);
  std::cout << "Waiting on poll().." << std::endl;
  //do
  while (poll(fds, nfds, timeout) > 0)
  {
	    std::cout << "i got from poll().." << std::endl;
   // rc = poll(fds, nfds, timeout);
/*
    if (rc < 0)
    {
      perror("  poll() failed");
      break;
  }*/
   /* if (rc == 0)
    {
      printf("  poll() timed out.  End program.\n");
      break;
  }*/
    current_size = nfds;
    for (i = 0; i < current_size; i++)
    {
      if(fds[i].revents == 0)
        continue;

	  /*if(fds[i].revents != POLLIN && fds[i].revents != POLLOUT )
      {
		  printf("  Error! revents = %d\n", fds[i].revents);
        end_server = TRUE;
        break;

	}*/
      if (fds[i].fd == listen_sd)
      {
        std::cout << "Listening socket is readable from " << listen_sd << std::endl;
        while (true)
        {
			struct sockaddr_in empty_addr;
			int addr_s;
			addr_s = sizeof(addr_s);
			new_sd = accept(listen_sd, (struct sockaddr *)&empty_addr, (socklen_t*)&addr_s);

			if (errno == EWOULDBLOCK)
			{
				std::cout << "got all connections" << std::endl;
				errno = 0;
				break;
			}
			/*if (new_sd < 0)
			{
				if (errno != EWOULDBLOCK)
					{
					perror("  accept() failed");
					end_server = TRUE;
					}
				break;
			}*/
			std::cout << "  New incoming connection - " << std::to_string(new_sd) << std::endl;
			fds[nfds].fd = new_sd;
			fds[nfds].events = POLLIN;
			nfds++;
		}
      }

      else
      {
        std::cout << "Descriptor "<< std::to_string(fds[i].fd) << " is readable" << std::endl;
        close_conn = FALSE;
        while(TRUE)
        {
			if (fds[i].revents & POLLIN)
			{
				std::cout << "IM GETTING INSIDE and i is" << i << std::endl;
				rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				std::cout << "recv here done here: " << buffer << std::endl;
				//should  be different and in connection to fd may  be
				req.parse_request(buffer);
				resp.make_response(config, req);

				if (rc == 0)
				{
					std::cout << "Connection closed" << std::endl;
					close_conn = TRUE;
					break;
				}
				len = rc;

				//fds[i].revents = 0;
				fds[i].revents = POLLOUT; // if recv is over -> else stay on
				std::cout << std::to_string(len) << "bytes received" << std::endl;
			}
			if (fds[i].revents == POLLOUT)
			{
				  		   // // Отправляем данные обратно клиенту
				  		   // int send_bytes = send(*it, resp.getRespons().c_str(), resp.getRespons().length(), MSG_DONTROUTE);
				int rc = send(fds[i].fd, resp.getRespons().c_str(), \
					resp.getRespons().length(), 0);
				if (rc < 0)
				{
					perror("");
					close_conn = TRUE;
					break;
				}
				//fds[i].revents = 0;
				//fds[i].events = 0;
				//close_conn = TRUE;
				fds[i].revents = 0; //if send is not over repeat send
			}
			break;
		}
		if (close_conn)
		{
		  std::cout << "ceaning proc started on " << i << "and fd: " << fds[i].fd << std::endl;
		  close(fds[i].fd);
		  fds[i].fd = -1;
		  compress_array = TRUE;
		}
	}  /* End of existing connection is readable             */
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
