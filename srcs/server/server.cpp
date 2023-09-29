/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:23:51 by crisfern          #+#    #+#             */
/*   Updated: 2023/09/29 13:51:56 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <netinet/in.h>

// CONSTRUCTORS
server::server( void )
{
	std::cout << "Server Default constructor called" << std::endl;
}

server::server( std::string port , std::string pass ) : _active_fds(1) // Tested
{	
	std::cout << "Server Parameter constructor called" << std::endl;
	this->data.host 		= SERVER_HOST;
	this->data.port 		= port;
	this->data.pass 		= pass;
	this->init_list_of_cmds();
	this->server_socket = new autosocket(this->data.port, this->data.host);
}

server::server( std::string network , std::string port , std::string pass ) : _active_fds(1)
{	
	std::cout << "Server Parameter constructor called" << std::endl;
	std::vector <std::string>seglist = ft_split(network, ':');
	this->data.host 		= seglist[0];
	this->data.network_port = seglist[1];
	this->data.network_pass = seglist[2];
	this->data.port 		= port;
	this->data.pass 		= pass;
	this->init_list_of_cmds();
	this->server_socket = new autosocket(this->data.port, this->data.host);
}

server::server( const server & var ) {	// No test
	(void)var;
	std::cout << "Server Copy constructor called" << std::endl;

}

// DESTRUCTOR
server::~server( void ) { // No test

	std::cout << "Server Destructor constructor called" << std::endl;
	delete this->server_socket;
}

// OVERLOADING
server & server::operator=(const server &tmp)	// No test
{
	(void)tmp;
	std::cout << "Server Operator equalizer called" << std::endl;
	return *this;
}

std::ostream &operator<<(std::ostream& os, const server &tmp) // Tested
{
	os << "host           |     " << tmp.get_host() << std::endl;
	os << "password       |     " << tmp.get_password() << std::endl;
	os << "port           |     " << tmp.get_port() << std::endl;
	return (os);
}

// FUNCTIONS
void	server::init_list_of_cmds(void) // Update with new commands Tested
{
	this->list_of_cmds.insert(std::pair<std::string, command_function>("NICK", &cmd::nick));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("USER", &cmd::username));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("QUIT", &cmd::quit));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("PRIVMSG", &cmd::privmsg));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("JOIN", &cmd::join));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("TOPIC", &cmd::topic));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("INVITE", &cmd::invite));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("KICK", &cmd::kick));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("PART", &cmd::part));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("NOTICE", &cmd::notice));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("NAMES", &cmd::names));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("LIST", &cmd::list));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("MODE", &cmd::mode));
//	this->list_of_cmds.insert(std::pair<std::string, command_function>("PONG", &cmd::pong));
}

void	server::init_pollfd(void) // Tested
{
	memset(this->poll_fds, 0, sizeof(this->poll_fds));
	this->poll_fds[0].fd 	   = this->server_socket->fd;
	this->poll_fds[0].events   = POLLIN;
}

bool	server::wait_for_connection(void) // Tested
{
	int ret;

	this->init_pollfd();
	while (true)
	{
		ret = poll(this->poll_fds, this->_active_fds, TIMEOUT); //TODO cambiar timeout + check ping clients
		if (ret < 0) {
			perror("Poll error");
			return 1;
		}
		if (ret == 0)
			continue;
		if (this->fd_ready() == 1)
			return 1;
		// ping  users and disconnect inactive
	}
	return 0;
}

bool	server::fd_ready(void)
{
	for (int i = 0; i < this->_active_fds; i++)
	{
		if (this->poll_fds[i].revents == 0)
			continue;
		if (this->poll_fds[i].fd == this->server_socket->fd)
		{
			this->accept_communication();
			return 0;
		}
		else
		{
			this->receive_communication(i);
			return 0;
		}
	}
	return 1;
}

void	server::add_user(int fd, sock_in client_addr) // Tested
{
	char ip_address[20];

	this->poll_fds[this->_active_fds].fd = fd;
	this->poll_fds[this->_active_fds].events = POLLIN;
	this->_active_fds++;
	user 	new_user(fd, inet_ntop(AF_INET, &(client_addr.sin_addr), ip_address, sizeof(ip_address)));
	this->list_of_users.insert(std::pair<int, user>(fd, new_user));
}

bool	server::accept_communication(void) // No test
{
	int 	fd;
	sock_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);

	fd = accept(this->server_socket->fd, (sock_addr*)&client_addr, &client_addr_size);
	if (fd < 0)
    {
        if (errno != EWOULDBLOCK)
          perror("  accept() failed");
    	return 1;
    }
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror(" FCNTL failed");
		return 1;
	}
	std::cout << this->_active_fds << std::endl;
	if (this->_active_fds <= MAX_CLIENTS)
	{
		this->add_user(fd, client_addr);
		std::cout << YELLOW << "Accepted communication from: " << fd << RESET << std::endl;
	}
	else
		close(fd);
	return 0;
}

bool	server::receive_communication(int poll_fd_pos) // No test
{
	char buffer[MSG_SIZE];
	int len;

	std::cout << "Message received" << std::endl;
	memset(buffer, 0, MSG_SIZE); //Iniciar buffer con ceros porque mete mierda
	len = recv(this->poll_fds[poll_fd_pos].fd, buffer, sizeof(buffer), 0);
	if (len < 0)
    {
		if (errno != EWOULDBLOCK)
			perror("  recv() failed");
        exit(1);
    }
    if (len == 0)
    {
		std::cout << "  Connection closed" << std::endl;
		// Close fd >> Delete fd from poll >> Delete user from list_of_users
		this->delete_user(poll_fd_pos);
		return 0;
    }
	buffer[len-1] = 0; //El intro lo ponemos a cero
	if (buffer[0] != 0)
	{
		std::multimap<std::string, std::string> commands = this->parse_message(buffer);
		this->execute_commands(poll_fd_pos, commands);
	}
	return 0;
}

bool	server::send_message(std::string msg, int fd) // No test
{
	int len = send(fd, msg.c_str(), msg.length(), 0);
	if (len < 0)
    {
		if (errno == 9)
			std::cout << RED << "(mocking message) " << RESET << msg << std::endl;
		else
			perror("  send() failed");
		return 1;
    }
	return 0;
}

void	server::delete_user(int poll_fd_pos) // Tested
{
	std::cout << RED << "Deleted user: fd " << this->poll_fds[poll_fd_pos].fd
		<< RESET << std::endl;
	close(this->poll_fds[poll_fd_pos].fd);
	this->list_of_users.erase(this->poll_fds[poll_fd_pos].fd);
	for (int count = poll_fd_pos; count <= this->_active_fds - 1; count++)
		this->poll_fds[count] = this->poll_fds[count + 1];
	this->poll_fds[this->_active_fds - 1].fd = 0;
	this->poll_fds[this->_active_fds - 1].events = 0;
	this->_active_fds--;
	//this->poll_fds[poll_fd_pos].fd = -1;
}

// Separa la cadena en COMANDO + MSG, donde mensaje es todo lo demás que es parseado de forma distinta por cada comando
std::multimap<std::string, std::string> server::parse_message(std::string msg) // Tested
{
	// Este split es por culpa de irssi, que lanza todos los comandos NICK y USER en una sola linea
	// No deberia afectar a los usuarios que lanzan comandos de uno en uno
	std::vector<std::string> seglist = ft_split(msg, '\n');
	std::vector<std::string>::iterator v_it;
	std::multimap<std::string, std::string> commands;
	std::string cmd;
	std::string args;
	size_t ind;

	for (v_it = seglist.begin(); v_it != seglist.end(); v_it++)
	{
		ind = v_it->find(" ");
		cmd = v_it->substr(0, ind);
		if (ind != std::string::npos)
			args= v_it->substr(ind + 1);
		commands.insert(std::pair<std::string, std::string>(cmd, args));
	}
	return commands;
}

void	server::execute_commands(int poll_fd_pos, std::multimap<std::string, std::string> commands) // No test
{
	poll_fd pollfd = this->get_pollfd(poll_fd_pos);
	user &usr = this->get_user(pollfd.fd);
	std::multimap<std::string, std::string>::iterator it;

	for (it = commands.begin(); it != commands.end(); it++)
	{
		if (this->list_of_cmds[it->first])
		{
			this->list_of_cmds[it->first](*this, poll_fd_pos, it->second);
		}
		else if (usr.get_is_registered())
			this->send_message(ERR_UNKNOWNCOMMAND(it->first), usr.get_fd());
	}
}

void	server::create_channel(user &usr, std::string name, std::string password) // No test
{
	// if (name[0] != '#' && name[0] != '&')
	// {
	// 	std::string channel_mark("#");
	// 	name.insert(0, channel_mark);
	// }
	channel cnn(name, password);
	if (!password.empty())
		cnn.set_mode("k");
	usr.set_op(true);
	cnn.add_member(usr);
	usr.set_op(false);
	this->list_of_channels.insert(std::pair<std::string, channel>(name, cnn));
	std::cout << name << " channel created!" << std::endl;
	std::cout << cnn << std::endl;
}

// Maybe make a template????
user *server::get_user_from_nick(std::string nick) // No test
{
	std::map<int, user>::iterator it;

	for (it = this->list_of_users.begin(); it != this->list_of_users.end(); it++)
	{
		if (it->second.get_nick().compare(nick) == 0)
			return &(it->second);
	}
	return NULL;
}

channel *server::get_channel_from_name(std::string name) // No test
{
	std::map<std::string, channel>::iterator it;

	for (it = this->list_of_channels.begin(); it != this->list_of_channels.end(); it++)
	{
		if (it->second.get_name().compare(name) == 0)
			return &(it->second);
	}
	return NULL;
}

void	print_poll_fd(int active_fds, poll_fd *poll_fds)
{
	std::cout << "Poll fd:" << std::endl;
	for (int i = 0; i < active_fds; i++)
	{
		std::cout << i << " - " << "fd " << poll_fds[i].fd << ", "
			<< "events " << poll_fds[i].events <<  ", "
 			<< "revents " << poll_fds[i].revents << std::endl;
	}
	std::cout << std::endl;
}

struct sockaddr_in	build_address(char *url, int port)
{
	struct sockaddr_in myaddr;

	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	inet_aton(url, &myaddr.sin_addr);

	return myaddr;
}

void	print_list_of_users(std::map<int, user> &list_of_users)
{
	std::map<int, user>::iterator it = list_of_users.begin();
	int fd;
	std::cout << "List of users:" << std::endl;
	for (; it != list_of_users.end(); it++)
	{
		fd = it->first;
			std::cout << "User fd: " << fd << std::endl;
			std::cout << "------------" << std::endl;
			std::cout << list_of_users[fd] << std::endl;
	}
}
void	test_connection(server *serv)
{
	std::cout << YELLOW << "CONNECT\n" << "Open a new terminal and type nc -v 127.0.0.1 6776 to test new connection.\n"
		<< RESET << serv->wait_for_connection() << std::endl;
}

/*###########################################
#				   GETTERS					#
############################################*/

std::string server::get_host(void) const
{
	return(this->data.host);
}

std::string server::get_port(void) const
{
	return(this->data.port);
}

std::string server::get_password(void) const
{
	return(this->data.pass);
}

user& 	server::get_user(int i)
{
	return(this->list_of_users.find(i)->second);
}

pollfd&	server::get_pollfd(int i)
{
	return (this->poll_fds[i]);
}

std::map<int, user> server::get_list_of_users(void) const
{
	return(this->list_of_users);
}

std::map<std::string, channel> server::get_list_of_channels(void) const
{
	return(this->list_of_channels);
}
