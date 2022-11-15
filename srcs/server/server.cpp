/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 15:15:25 by mortiz-d          #+#    #+#             */
/*   Updated: 2022/11/09 13:34:23 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <string>
#include <vector>
#include <sstream>

server::server( void ) : host(""), network_pass (""), network_port(""), port(""), password("")
{

  std::cout << "Default constructor called" << std::endl;

}

server::server( std::string network , std::string prt , std::string pass ) : host(""), network_port (""), network_pass(""), port(""), password("")
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
    this->port          = prt;
	this->password      = pass;
  std::cout << "Parameter constructor called" << std::endl;

}

server::server( const server & var ) {

  std::cout << "Copy constructor called" << std::endl;

}

server::~server( void ) {

  std::cout << "Destructor constructor called" << std::endl;

}

server & server::operator=(const server &tmp)
{
  std::cout << "Operator equalizer called" << std::endl;
  return *this;
}

std::ostream &operator<<(std::ostream& os, const server &tmp)
{
	os << "Operator output called" << std::endl;
	os << "host           |     " << tmp.get_host() << std::endl;
	os << "network pass   |     " << tmp.get_network_pass() << std::endl;
	os << "network port   |     " << tmp.get_network_port() << std::endl;
	os << "password       |     " << tmp.get_password() << std::endl;
	os << "port           |     " << tmp.get_port() << std::endl;
	return (os);
}


=======
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
  if (port_i < 6000 /*|| port_i > 7000*/) // What about 80 and 8080 port????????
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

int		server::connect_to_host(void)
{
	sock_in  addr;

	if ((this->host_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return 0;
	std::cout << "Init socket" << std::endl;
	addr = init_socket_struct(this->network_port, this->host);
	if (connect(this->host_socket, (const sock_addr*)&addr, sizeof(addr)) == -1)
	{
		perror("Connection failed");
		return 0;
	}
	std::cout << "Connected!" << std::endl;
	return 1;
}

int		server::server_listening(void)
{
	sock_in	addr;
	int		opt = 1;

	if ((this->client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return 0;
	std::cout << "Init socket" << std::endl;
	// No sé que opciones tendremos que habilitar pero vamos a tener que usarlo
	if (setsockopt(this->client_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		perror("Bad socket ");
		return 0;
	}
	addr = init_socket_struct(this->network_port, this->host);
	// Asigna un nombre al socket; Asigna la info de address al socket
	if (bind(this->client_socket, (const sock_addr*)&addr, sizeof(addr)) == -1)
	{
		perror("Error binding ");
		return 0;
	}
	// Tenemos que definir un max_size para la cola
	if (listen(this->client_socket, 5) == -1)
	{
		perror("Can't hear you");
		return 0;
	}
	return 1;
}
