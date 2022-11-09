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

server::server( void ) {

  std::cout << "Default constructor called" << std::endl;
  
}

server::server( std::string str ) {

  std::cout << "Parameter constructor called" << std::endl;
  
}

server::server( const server & var ) {

  std::cout << "Copy constructor called" << std::endl;
  
}

server::~server( void ) {
  
  std::cout << "Destructor constructor called" << std::endl;

}

server server::&operator=(const server &tmp)
{
  std::cout << "Operator equalizer called" << std::endl;
}

std::ostream &operator<<(std::ostream& os, const server &tmp)
{
	std::cout << "Operator output called" << std::endl;
	return (os);
}

