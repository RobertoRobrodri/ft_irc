#include "server/server.hpp"
#include "command/command.hpp"
#include <iostream>
#include <iomanip>
#include <unistd.h>

int main()
{
	int test = -1;
	char *port = "6776";
	char *pass = "pass";
	server *serv;
	while (true)
	{
		std::cout << "Which test would you like to run?\n" <<
			"  1) Server construction\n" <<
			"  2) Add user\n" <<
			"  3) Delete user\n" <<
			"  4) Getters\n" <<
			"  5) Message parser\n" <<
			"  6) USER\n" <<
			"  7) NICK\n" <<
			"  8) JOIN\n" <<
			"  9) PONG\n" <<
			"  10) QUIT\n" <<
			"  11) PRIVMSG\n" <<
			"  12) TOPIC\n" <<
			"  13) INVITE\n" <<
			"  14) KICK\n" <<
			"  15) [Exit tester]\n";
		std::cin >> test;
		
		if (test > 1)
		{
			serv = new server(port, pass);
			serv->init_pollfd();
		}
		switch (test)
		{
			case 1:
				test_server_construction(port, pass);
				break;
			case 2:
				test_add_user(serv, 4, "63.161.169.138", 3490);
				test_add_user(serv, 5, "63.161.169.137", 3491);
				test_add_user(serv, 6, "63.161.169.138", 3491);
				serv->delete_user(1);
				serv->delete_user(2);
				serv->delete_user(3);
				break;
			case 3:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				test_delete_user(serv, 1);
				break;
			case 4:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				test_getters(serv);
				serv->delete_user(2);
				break;
			case 5:
				test_parse_message(serv, "USER paco");
				break;
			case 6:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				serv->add_user(5, build_address("63.161.169.137", 3491));
				test_user_cmd(serv); 
				serv->delete_user(1);
				serv->delete_user(2);
				break;
			case 7:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				serv->add_user(5, build_address("63.161.169.137", 3491));
				test_nick_cmd(serv);
				serv->delete_user(1);
				serv->delete_user(2);
				break;
			// fd_ready? execute_command?
			case 8:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				test_join_cmd(serv);
				serv->delete_user(1);
				break;
			case 9:
			// pong
				break;
			case 10:
			// quit
				break;
			case 11:
			// privmsg
				break;
			case 12:
			// topic
				break;
			case 13:
			// invite
				break;
			case 14:
			// kick	
				break;
			case 15:
				std::cout << MAG << "See you next time!\n";
				return 0;
		}
		delete serv;
		test = -1;
	}
	return (0);
}
