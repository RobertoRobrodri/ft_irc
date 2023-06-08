#include "server.hpp"
#include <string>
#include <vector>
#include <sstream>

// CONSTRUCTORS

server::server( void ) : host(""), network_pass (""), network_port(""), port(""), password("")
{

	std::cout << "Default constructor called" << std::endl;

}

server::server( std::string network , std::string prt , std::string pass ) : active_fds(1), host(""), network_pass (""), network_port(""), port(prt), password(pass)
{
	std::stringstream test(network);
	std::string segment;
	std::vector <std::string>seglist;

	while (std::getline(test,segment,':'))
		seglist.push_back(segment);

	if (seglist.size() == 3)
	{
		this->host          = seglist[0];
		this->network_port  = seglist[1];
		this->network_pass  = seglist[2];
	}
	this->server_socket = new autosocket(this->port, this->host);
	std::cout << "Parameter constructor called" << std::endl;

}

server::server( const server & var ) {
	(void)var;
	std::cout << "Copy constructor called" << std::endl;

}

// DESTRUCTOR
server::~server( void ) {

	std::cout << "Destructor constructor called" << std::endl;
	delete this;

}

// OVERLOADING
server & server::operator=(const server &tmp)
{
	(void)tmp;
	std::cout << "Operator equalizer called" << std::endl;
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
bool	server::is_good_host(std::string host) const
{
	std::stringstream test(host);
	std::string segment;
	std::vector <int>seglist;

	while (std::getline(test,segment,'.'))
	{
		if (segment.c_str() != std::to_string(atoi(segment.c_str())))
			seglist.push_back(-1);
		else
			seglist.push_back(atoi(segment.c_str()));
	}
	if (seglist.size() != 4)
		return (0);
	for (int i = 0; i < 4; i++)
		if (seglist[i] < 0 || seglist[i] > 255)
			return (0);
	return (1);
}


bool	server::is_good_port(std::string port) const
{
	int port_i;

	port_i = atoi(port.c_str());
	if (port != std::to_string(port_i))
		return(0);
	if (port_i < 6000 || port_i > 7000)
		return (0);
	return (1);
}

bool	server::check_data_correct(void) const
{
	if (this->host == "" || !this->is_good_host(this->host))
		return (0);
	if (this->network_pass == "")
		return (0);
	if (this->network_port == "" || !this->is_good_port(this->network_port))
		return (0);
	if (this->password == "")
		return (0);
	if (this->port == "" || !this->is_good_port(this->port))
		return (0);
	return (1);
}

bool	server::wait_for_connection(void)
{
	int ret;
	// Init pollfd struct
	memset(this->poll_fds, 0, sizeof(this->poll_fds));
	this->poll_fds[0].fd 	   = this->server_socket->fd;
	this->poll_fds[0].events   = POLLIN;

	while (true)
	{
		std::cout << "Dead server 💀💀💀💀 fr fr no cap" << std::endl;
		std::cout << this->poll_fds[0].fd << " " << this->active_fds << std::endl;
		ret = poll(this->poll_fds, this->active_fds, TIMEOUT);
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
	for (int i = 0; i < this->active_fds; i++)
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
	int 	new_socket = 0;

	new_socket = accept(this->server_socket->fd, NULL, NULL);
	if (new_socket < 0)
    {
        if (errno != EWOULDBLOCK)
          perror("  accept() failed");
		std::cout << "Ok Schizo " << std::endl;
    	return 1;
    }
	// TODO Gestionar fds cerrados -  sockets muertos
	std::cout << "Listening socket is readable fr fr no cap" << std::endl;
	this->poll_fds[this->active_fds].fd = new_socket;
	this->poll_fds[this->active_fds].events = POLLIN;
	this->active_fds++;
	std::cout << "New socket: " << new_socket << std::endl;
	std::cout << "Active clients: "<< this->active_fds << std::endl;
	
	return 1;
}

bool	server::receive_communication(int i)
{
	char *buffer[MSG_SIZE];
	int len;

	std::cout << "Message received" << std::endl;
	len = recv(this->poll_fds[i].fd, buffer, sizeof(buffer), 0);
	if (len < 0)
    {
		if (errno != EWOULDBLOCK)
			perror("  recv() failed");
        return 1;
    }
    if (len == 0)
    {
		std::cout << "  Connection closed" << std::endl;
		close(this->poll_fds[i].fd);
		this->poll_fds[i].fd = -1;
		this->active_fds--;
		return 1;
    }
	len = send(this->poll_fds[i].fd, buffer, len, 0);
	if (len < 0)
    {
		perror("  send() failed");
		return 1;
    }
	return 0;
}