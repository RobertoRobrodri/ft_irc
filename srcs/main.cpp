/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:25:59 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/23 15:28:31 by mzomeno-         ###   ########.fr       */
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
	std::string host = "127.0.0.1";
	std::string port = "6776";

	//atexit(leaks);
	try {
		if (check_data_correct(argv, argc) == false)
			throw std::runtime_error("Data incorrect");
		if (argc == 1)
			serv = new server("6776", "pass");
		if (argc == 3)
		{
			port = argv[1];
			serv = new server(port, argv[2]);
		}
		if (argc == 4)
		{
			std::string network(argv[1]);
			std::size_t pos = network.find(":");
			host = network.substr(0, pos);
			port = argv[2];
			serv = new server(network, port, argv[3]);
		}
		std::cout << YELLOW << "Server initialized!\n" <<
			"To test new connections, open a new tab and type nc -vc " <<
			host << " " << port << "\n" << RESET;
		if (serv->wait_for_connection())
			throw std::runtime_error("Conection failed");
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
	delete serv;
	return (0);
}
