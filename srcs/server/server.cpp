#include "server.hpp"
#include <string>
#include <vector>
#include <sstream>

// CONSTRUCTORS

server::server( void )
{
	std::cout << "Server Default constructor called" << std::endl;
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

server::server( const server & var )
{
	(void)var;
	std::cout << "Server Copy constructor called" << std::endl;
}

// DESTRUCTOR
server::~server( void )
{
	std::cout << "Server Destructor constructor called" << std::endl;
	delete this;
}

// OVERLOADING
server & server::operator=(const server &tmp)
{
	(void)tmp;
	std::cout << "Server Operator equalizer called" << std::endl;
	return *this;
}

std::ostream &operator<<(std::ostream& os, const server &tmp)
{
	os << "host           |     " << tmp.get_host() << std::endl;
	os << "network pass   |     " << tmp.get_network_pass() << std::endl;
	os << "network port   |     " << tmp.get_network_port() << std::endl;
	os << "password       |     " << tmp.get_password() << std::endl;
	os << "port           |     " << tmp.get_port() << std::endl;
	return (os);
}

// FUNCTIONS
void	server::init_list_of_cmds(void)
{
	this->list_of_cmds.insert(std::pair<std::string, command_function>("NICK", &cmd::nick));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("USER", &cmd::username));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("PONG", &cmd::pong));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("QUIT", &cmd::quit));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("PRIVMSG", &cmd::privmsg));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("JOIN", &cmd::join));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("TOPIC", &cmd::topic));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("INVITE", &cmd::invite));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("KICK", &cmd::kick));
}

void	server::init_pollfd(void)
{
	memset(this->poll_fds, 0, sizeof(this->poll_fds));
	this->poll_fds[0].fd 	   = this->server_socket->fd;
	this->poll_fds[0].events   = POLLIN;
}

bool	server::wait_for_connection(void)
{
	int ret;
	
	this->init_pollfd();
	std::cout << GREEN << "Server initialized! Waiting for connections..." << RESET << std::endl;
	while (true)
	{
		ret = poll(this->poll_fds, this->_active_fds, TIMEOUT); //TODO check ping clients
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


int	server::fd_ready( void )
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

void	server::add_user(int fd, sock_in client_addr)
{
	char ip_address[20];

	this->poll_fds[this->_active_fds].fd = fd;
	this->poll_fds[this->_active_fds].events = POLLIN;
	this->_active_fds++;
	user 	new_user(fd, inet_ntop(AF_INET, &(client_addr.sin_addr), ip_address, sizeof(ip_address)));
	this->list_of_users.insert(std::pair<int, user>(fd, new_user));
}

bool	server::accept_communication(void)
{
	int 	fd = 0;
	sock_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	char ip_addres[20];
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
	std::cout << YELLOW << "Accepted communication from: " << fd << RESET << std::endl;
	this->add_user(fd, client_addr);
	return 0;
}

bool	server::receive_communication(int poll_fd_pos)
{
	char buffer[MSG_SIZE];
	int len;

	std::cout << "Message received" << std::endl;
	memset(buffer, 0, MSG_SIZE); //Iniciar buffer con ceros porque mete mierda
	len = recv(this->poll_fds[poll_fd_pos].fd, buffer, sizeof(buffer), 0);
	std::cout << GREEN << buffer << RESET <<std::endl;
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
		this->parse_message(poll_fd_pos, buffer);
	return 0;
}

bool server::send_message(std::string msg, int fd)
{
	int len = send(fd, msg.c_str(), msg.length(), 0);
	if (len < 0)
    {
		perror("  send() failed");
		return 1;
    }
	return 0;
}

void	server::delete_user(int poll_fd_pos)
{
	std::cout << RED << "Deleted user: " << RESET << std::endl;
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
void	server::parse_message(int poll_fd_pos, std::string msg)
{
	// Este split es por culpa de irssi, que lanza todos los comandos NICK y USER en una sola linea
	// No deberia afectar a los usuarios que lanzan comandos de uno en uno
	cmd_map::iterator it;
	std::vector<std::string> seglist = ft_split(msg, '\n');
	std::vector<std::string>::iterator v_it;

	for (v_it = seglist.begin(); v_it != seglist.end(); v_it++)
	{
		int ind = v_it->find(" ");
		std::string cmd = v_it->substr(0, ind);
		*v_it = v_it->substr(ind + 1);
		std::cout << CYAN << *v_it << RESET << std::endl;
		it = this->list_of_cmds.find(cmd);
		if (it != this->list_of_cmds.end())
			it->second(*this, poll_fd_pos, *v_it);
	}
}

void	server::create_channel(user &usr, std::string name)
{
	// IRSSI:  si el server NO empiza por #, lo añade
	// Puede contener # o & entre medias
	//if (name[0] != '#' || name[0] != '&')
	//	name.insert(0, "#");

	channel cnn(name);
	cnn.add_member(usr);
	this->list_of_channels.insert(std::pair<std::string, channel>(name, cnn));
	std::cout << YELLOW << name << " channel created!" << std::endl;
	std::cout << cnn << RESET << std::endl;
}

user& 	server::get_user(int i)
{
	return(this->list_of_users.find(i)->second);
}

pollfd&	server::get_pollfd(int i)
{
	return (this->poll_fds[i]);
}

// Maybe make a template????
user *server::get_user_from_nick(std::string nick)
{
	std::map<int, user>::iterator it;

	for (it = this->list_of_users.begin(); it != this->list_of_users.end(); it++)
	{
		if (it->second.get_nick().compare(nick) == 0)
			return &(it->second);
	}
	return NULL;
}

channel *server::get_channel_from_name(std::string name)
{
	std::map<std::string, channel>::iterator it;
	std::string tmp(name);

	for (it = this->list_of_channels.begin(); it != this->list_of_channels.end(); it++)
	{
		if (it->second.get_name().compare(tmp) == 0)
			return &(it->second);
	}
	return NULL;
}
