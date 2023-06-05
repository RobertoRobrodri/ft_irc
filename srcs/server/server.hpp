#ifndef SERVER_HPP
#define SERVER_HPP
#define	sock_in		struct sockaddr_in
#define	sock_addr	struct sockaddr
#define	poll_fd		struct pollfd
#define	MAX_CLIENTS	5
#define	TIMEOUT		-1
#define MSG_SIZE	80
#include <string>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

class	server {

	private:
		int 				active_fds;

		int					server_socket;
		int 				client_socket[MAX_CLIENTS];
		poll_fd				poll_fds[MAX_CLIENTS];

		std::string 		host;
		std::string 		network_pass;
		std::string 		network_port;
		std::string 		port;
		std::string 		password;

		server	( void );

		bool	is_good_port(std::string port) const;
		bool	is_good_host(std::string host) const;

		int		fd_ready(void);
		bool	accept_communication(void);
		bool	receive_communication(int i);
	public:

		server				( std::string network , std::string prt , std::string pass );
		server 				( const server & var );
		~server 			( void );
		server &operator=	(const server &tmp);

		/*###########################################
		#				GETTERS						#
		############################################*/
		std::string get_host(void) const 			{return(this->host);};
		std::string get_network_pass(void) const 	{return(this->network_pass);};
		std::string get_network_port(void) const 	{return(this->network_port);};
		std::string get_port(void) const 			{return(this->port);};
		std::string get_password(void) const 		{return(this->password);};

		/*###########################################
		#				FUNCTIONS					#
		############################################*/
		bool	check_data_correct(void) const;
		bool	server_listening(void);
		bool	wait_for_connection(void);
};

std::ostream &operator<<(std::ostream& os, const server &tmp);
sock_in	init_socket_struct(std::string port, std::string host);

#endif
