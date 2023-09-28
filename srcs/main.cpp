/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:25:59 by crisfern          #+#    #+#             */
/*   Updated: 2023/09/28 11:50:06 by crisfern         ###   ########.fr       */
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
	char *port = "6776";
	char *pass = "pass";
	server *serv;

	atexit(leaks);
	if (argc != 4)
		return (exit_msg("Unexact number of argvs"));
	try {
		if (check_data_correct(argv) == 0)
			throw std::runtime_error("Data incorrect");
		serv = new server(port, pass);
		if (serv->wait_for_connection())
			throw std::runtime_error("Conection failed");
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
	delete serv;
	return (0);
}
