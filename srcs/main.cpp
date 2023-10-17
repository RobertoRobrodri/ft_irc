/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:25:59 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/17 17:03:04 by mzomeno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/server.hpp"

void leaks()
{
	system("leaks ircserv");
}

int exit_msg(std::string msg)
{
	std::cout << msg << std::endl;
	return (0);
}

int main(int argc, char **argv)
{
	server *serv;

	atexit(leaks);
	try {
		if (argc == 4)
		{
			if (check_data_correct(argv) == 0)
				throw std::runtime_error("Data incorrect");
			serv = new server(argv[1], argv[2], argv[3]);
			std::cout << YELLOW << "Server initialized!\n" <<
				"To test new connections, open a new tab and type nc -vc <host> <port>\n"
				<< RESET;
		}
		else
		{
			serv = new server("6776", "pass");
			std::cout << YELLOW << "Server initialized!\n" <<
				"To test new connections, open a new tab and type nc -vc 127.0.0.1 6776\n"
				<< RESET;
		}
		if (serv->wait_for_connection())
			throw std::runtime_error("Conection failed");
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
	delete serv;
	return (0);
}
