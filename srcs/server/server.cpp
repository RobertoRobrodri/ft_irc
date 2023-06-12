#include "server.hpp"
#include <string>
#include <vector>
#include <sstream>

// CONSTRUCTORS

server::server( void )
{

	std::cout << "Server Default constructor called" << std::endl;

}

server::server( std::string network , std::string port , std::string pass ) : _active_fds(1) {
	
	std::cout << "Server Parameter constructor called" << std::endl;
	std::vector <std::string>seglist = ft_split(network, ':');
	this->data.host 		= seglist[0];
	this->data.network_port = seglist[1];
	this->data.network_pass = seglist[2];
	this->data.port 		= port;
	this->data.pass 		= pass;
	this->server_socket = new autosocket(this->data.port, this->data.host);

}

server::server( const server & var ) {
	(void)var;
	std::cout << "Server Copy constructor called" << std::endl;

}

// DESTRUCTOR
server::~server( void ) {

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

bool	server::wait_for_connection(void)
{
	int ret;
	// Init pollfd struct
	memset(this->poll_fds, 0, sizeof(this->poll_fds));
	this->poll_fds[0].fd 	   = this->server_socket->fd;
	this->poll_fds[0].events   = POLLIN;

	while (true)
	{
		std::cout << "IRC 💀💀💀💀 IRC" << std::endl;
		ret = poll(this->poll_fds, this->_active_fds, TIMEOUT);
		// for (int i = 0; i < this->_active_fds; i++)
		// 	std::cout << this->poll_fds[i].fd << std::endl;
		if (ret < 0) {
			perror("Poll error");
			return 1;
		}
		// En principio no hay tiemout TIMEOUT = -1
		// if (ret == 0) {
		// 	perror("Timeout");
		// 	return 1;
		// }
		if (this->fd_ready() == 1)
			return 1;
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

bool	server::accept_communication(void)
{
	int 	fd = 0;

	fd = accept(this->server_socket->fd, NULL, NULL);
	if (fd < 0)
    {
        if (errno != EWOULDBLOCK)
          perror("  accept() failed");
		std::cout << "Ok Schizo " << std::endl;
    	return 1;
    }
	std::cout << "Listening socket is readable fr fr no cap" << std::endl;
	this->poll_fds[this->_active_fds].fd = fd;
	this->poll_fds[this->_active_fds].events = POLLIN;
	this->_active_fds++;
	user 	new_user;
	this->list_of_users.insert(std::pair<int, user>(fd, new_user));
	// std::cout << "New socket: " << fd << std::endl;
	// std::cout << "Active clients: " << this->_active_fds << std::endl;
	// std::cout << "New user: " << std::endl;
	// std::cout << this->list_of_users[fd] << std::endl;
	
	return 0;
}

bool	server::receive_communication(int i)
{
	char buffer[MSG_SIZE];
	int len;

	std::cout << "Message received" << std::endl;
	len = recv(this->poll_fds[i].fd, buffer, sizeof(buffer), 0);
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
		this->delete_user(i);
		return 0;
    }
//	this->parse_message(i, buffer);
	//this->send_message(buffer, this->poll_fds[i].fd, len);
	return 0;
}

bool	server::send_message(char *msg, int fd, int len)
{
	len = send(fd, msg, len, 0);
	if (len < 0)
    {
		perror("  send() failed");
		return 1;
    }
	return 0;
}

void	server::delete_user(int i)
{
	std::cout << "Deleted user: " << std::endl;
	close(this->poll_fds[i].fd);
	this->list_of_users.erase(this->poll_fds[i].fd);
	for (int count = i; count <= this->_active_fds - 1; count++)
		this->poll_fds[count] = this->poll_fds[count + 1];
	this->poll_fds[this->_active_fds - 1].fd = 0;
	this->poll_fds[this->_active_fds - 1].events = 0;
	this->_active_fds--;
	//this->poll_fds[i].fd = -1;
}
// void	server::parse_message(int i, std::string msg)
// {
// 	int j = 0;
// 	//TODO Hacerlo bien voy a asumir que los comandos están bien y extraer el que corresponde
// 	std::vector<std::string> seglist = ft_split(msg, ' ');
// 	this->cmd(seglist[0]);
// 	this->cmd->execute(i, this->list_of_users[i], seglist[1]);
// }