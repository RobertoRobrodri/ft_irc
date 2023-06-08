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
#include <unistd.h>
#include "../autosocket/autosocket.hpp"
#include "../parser/parser.hpp"

class	autosocket;

typedef struct Data_Server {             //Struct para almacenar los datos del servidor
	std::string host;
	std::string network_pass;
	std::string network_port;
	std::string port;
	std::string password;   
} data_server;

class	server {

	private:
		int 				_active_fds;

		autosocket			*server_socket;
		poll_fd				poll_fds[MAX_CLIENTS];

		std::string 		_host;
		std::string 		_network_pass;
		std::string 		_network_port;
		std::string 		_port;
		std::string 		_pass;

		server	( void );

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
		std::string get_host(void) const 			{return(this->_host);};
		std::string get_network_pass(void) const 	{return(this->_network_pass);};
		std::string get_network_port(void) const 	{return(this->_network_port);};
		std::string get_port(void) const 			{return(this->_port);};
		std::string get_password(void) const 		{return(this->_pass);};

		/*###########################################
		#				FUNCTIONS					#
		############################################*/
		bool	server_listening(void);
		bool	wait_for_connection(void);
};

std::ostream &operator<<(std::ostream& os, const server &tmp);

#endif
