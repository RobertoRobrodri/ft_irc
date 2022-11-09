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

server::server( void ) {

  std::cout << "Default constructor called" << std::endl;
  
}

server::server( std::string network , std::string prt , std::string pass )
{
    std::stringstream test(network);
    std::string segment;
    std::vector <std::string>seglist;

    while (std::getline(test,segment,':'))
      seglist.push_back(segment);

    this->host          = seglist[0];
    this->network_port  = seglist[1];
		this->network_pass  = seglist[2];
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
}

std::ostream &operator<<(std::ostream& os, const server &tmp)
{
	os << std::endl << "Operator output called" << std::endl;
  os << "host           |     " << tmp.get_host() << std::endl;
  os << "network pass   |     " << tmp.get_network_pass() << std::endl;
  os << "network port   |     " << tmp.get_network_port() << std::endl;
  os << "password       |     " << tmp.get_password() << std::endl;
  os << "port           |     " << tmp.get_port() << std::endl;
	return (os);
}

