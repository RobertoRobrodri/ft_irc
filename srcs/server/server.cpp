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

sock_in	init_socket_struct(std::string port, std::string host)
{
	sock_in	addr;
// Init struct that the socket needs

//  IPV4 addresses
	addr.sin_family				= AF_INET;
//  Convert our port to a network address (host to network)
	addr.sin_port				= htons(atoi(port.c_str()));
//  Our address as integer
	addr.sin_addr.s_addr		= inet_addr(host.c_str());
	return addr;
}

bool	server::server_listening(void)
{
	sock_in	addr;
	int		opt = 1;

	if ((this->server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return 0;
	std::cout << "Init socket" << std::endl;
	// No sé que opciones tendremos que habilitar pero vamos a tener que usarlo
	if (setsockopt(this->server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		perror("Bad socket ");
		return 0;
	}
	addr = init_socket_struct(this->network_port, this->host);
	// Asigna un nombre al socket; Asigna la info de address al socket
	if (bind(this->server_socket, (const sock_addr*)&addr, sizeof(addr)) == -1)
	{
		perror("Error binding ");
		return 0;
	}
	// Tenemos que definir un max_size para la cola
	if (listen(this->server_socket, MAX_CLIENTS) == -1)
	{
		perror("Can't hear you");
		return 0;
	}
	return 1;
}

bool	server::wait_for_connection(void)
{
	int ret;
	// Init pollfd struct
	memset(this->poll_fds, 0, sizeof(this->poll_fds));
	this->poll_fds[0].fd 	   = this->server_socket;
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
	int 	current_size = this->active_fds;

	for (int i = 0; i < current_size; i++)
	{
		if (this->poll_fds[i].revents == 0)
			continue;
		if (this->poll_fds[i].fd == this->server_socket)
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

	new_socket = accept(this->server_socket, NULL, NULL);
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

	std::cout << "Message receive" << std::endl;
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