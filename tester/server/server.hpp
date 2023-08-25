#ifndef SERVER_HPP
#define SERVER_HPP
#define	sock_in		struct sockaddr_in
#define	sock_addr	struct sockaddr
#define	poll_fd		struct pollfd
#define	MAX_CLIENTS	5
#define	TIMEOUT		-1
#define MSG_SIZE	512
#define SERVER_HOST	"127.0.0.1"

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
		bool								fd_ready(void);
		bool								accept_communication(void);
		bool								receive_communication(int i);
		std::map<std::string, std::string>	parse_message(std::string msg);
		void								init_list_of_cmds(void);
		void								init_pollfd(void);

		server				( std::string port , std::string pass );
		server 				( const server & var );
		~server 			( void );
		server &operator=	(const server &tmp);

		/*###########################################
		#				GETTERS						#
		############################################*/
		std::string get_host(void) const 			{return(this->data.host);};
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
		void	add_user(int fd, sock_in client_addr);
		void	delete_user(int i);
		void	execute_commands(int poll_fd_pos, std::map<std::string, std::string> commands);
		static bool	send_message(std::string msg, int fd);
		user	*get_user_from_nick(std::string nick);
		channel *get_channel_from_name(std::string name);
		void	create_channel(user &usr, std::string name);
};

std::ostream &operator<<(std::ostream& os, const server &tmp);


// TESTS
void	test_getters(server *serv);
void	test_check_data_correct();
void	test_server_construction(char *port, char *pass);
void	print_poll_fd(int active_fds, poll_fd *poll_fds);
void	print_list_of_users(std::map<int, user> &list_of_users);
struct sockaddr_in	build_address(char *url, int port);
void	test_add_user(server *serv, int fd, char *url, int port);
void	test_delete_user(server *serv, int fd_pos);
void	test_parse_message(server *serv, std::string msg);
void	test_connection(server *serv);

#endif
