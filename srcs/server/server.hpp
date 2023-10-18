/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:25:34 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/04 12:13:01 by mzomeno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#define	sock_in		struct sockaddr_in
#define	sock_addr	struct sockaddr
#define	poll_fd		struct pollfd
#define	MAX_CLIENTS	100
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

typedef struct t_Data_Server {             //Struct for store server data
	std::string host;
	std::string port;
	std::string pass;
	std::string network_port;
	std::string network_pass;
} data_server;

class	server {

	public:
		int 							_active_fds;
		autosocket						*server_socket;
		poll_fd							poll_fds[MAX_CLIENTS + 1]; // First position is our server
		data_server						data;
		cmd_map 						list_of_cmds;
		std::map<int, user> 			list_of_users;
		std::map<std::string, channel*> 	list_of_channels;

		server	( void );
		bool									fd_ready(void);
		bool									accept_communication(void);
		bool									receive_communication(int i);
		std::multimap<std::string, std::string>	parse_message(std::string msg);
		void									init_list_of_cmds(void);
		void									init_pollfd(void);

		server				( std::string port , std::string pass );
		server				( std::string network , std::string port , std::string pass );
		server 				( const server & var );
		~server 			( void );
		server &operator=	(const server &tmp);

		/*###########################################
		#				GETTERS						#
		############################################*/
		std::string get_host(void) const;
		std::string get_port(void) const;
		std::string get_password(void) const;
		user& 	get_user(int i);
		pollfd&	get_pollfd(int i);
		std::map<int, user> get_list_of_users(void) const;
		std::map<std::string, channel*> get_list_of_channels(void) const;

		/*###########################################
		#				FUNCTIONS					#
		############################################*/
		bool	wait_for_connection(void);
		void	add_user(int fd, sock_in client_addr);
		void	delete_user(int i);
		void	execute_commands(int poll_fd_pos, std::multimap<std::string, std::string> commands);
		static void	send_message(std::string msg, int fd);
		user	*get_user_from_nick(std::string nick);
		channel *get_channel_from_name(std::string name);
		void	create_channel(user &usr, std::string name, std::string password);

};

void	test_connection(server *serv);
std::ostream &operator<<(std::ostream& os, const server &tmp);

#endif
