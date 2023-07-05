#ifndef SERVER_HPP
#define SERVER_HPP
#define	sock_in		struct sockaddr_in
#define	sock_addr	struct sockaddr
#define	poll_fd		struct pollfd
#define	MAX_CLIENTS	5
#define	TIMEOUT		-1
#define MSG_SIZE	512

// COLORS
#define RESET "\033[1;0m"
#define BLACK "\e[0;30m"
#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define MAG "\e[0;35m"
#define CYAN "\e[0;36m"
#define WHITE "\e[0;37m"
// LIBRARIES
#include <string>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include "../autosocket/autosocket.hpp"
#include "../parser/parser.hpp"
#include "../user/user.hpp"
#include "../command/command.hpp"
#include "../channel/channel.hpp"

class	autosocket;
class 	user;
class 	context;
class   server;
class	channel;

typedef void (*command_function)(server &svr, int poll_fd_pos, std::string name);
typedef std::map<std::string, command_function> cmd_map;

typedef struct t_Data_Server {             //Struct para almacenar los datos del servidor
	std::string host;
	std::string network_pass;
	std::string network_port;
	std::string port;
	std::string pass;   
} data_server;

class	server {

	public:
		int 							_active_fds;
		autosocket						*server_socket;
		poll_fd							poll_fds[MAX_CLIENTS];
		data_server						data;
		cmd_map 						list_of_cmds;
		std::map<int, user> 			list_of_users;
		std::map<std::string, channel> 	list_of_channels;

		server	( void );
		int		fd_ready(void);
		bool	accept_communication(void);
		bool	receive_communication(int i);
		void	parse_message(int poll_fd_pos, std::string msg);
		void	init_list_of_cmds(void);

		server				( std::string network , std::string port , std::string pass );
		server 				( const server & var );
		~server 			( void );
		server &operator=	(const server &tmp);

		/*###########################################
		#				GETTERS						#
		############################################*/
		std::string get_host(void) const 			{return(this->data.host);};
		std::string get_network_pass(void) const 	{return(this->data.network_pass);};
		std::string get_network_port(void) const 	{return(this->data.network_port);};
		std::string get_port(void) const 			{return(this->data.port);};
		std::string get_password(void) const 		{return(this->data.pass);};
		user& 	get_user(int i);
		pollfd&	get_pollfd(int i);
		std::map<int, user> get_list_of_users(void) const {return(this->list_of_users);};
		std::map<std::string, channel> get_list_of_channels(void) const {return(this->list_of_channels);};

		/*###########################################
		#				FUNCTIONS					#
		############################################*/
		bool	wait_for_connection(void);
		void	delete_user(int i);
		bool	send_message(std::string msg, int fd);
		user	*get_user_from_nick(std::string nick);
		channel *get_channel_from_name(std::string name);
		void	create_channel(user &usr, std::string name);
};

std::ostream &operator<<(std::ostream& os, const server &tmp);

#endif

