#include "server/server.hpp"
#include "command/command.hpp"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <limits>
#include <unistd.h>
#include <string>

void	reset_cin()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main()
{
	int test_num;
	std::string input;
	char *port = "6776";
	char *pass = "pass";
	server *serv;
	while (true)
	{
		std::cout << BLUE << "Which test would you like to run?\n" << RESET <<
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
		std::cin >> input;
		if (std::cin.fail())
		{
			reset_cin();
			std::cin >> input;
		}
		test_num = std::stoi(input);
	
		if (test_num < 1 || test_num > 15)
		{
			std::cout << RED << "Please enter a valid value\n" << RESET;
			continue;
		}
		else if (test_num == 1)
		{
			test_server_construction(port, pass);
			std::cout << GREEN << "\nContinue...\n";
			reset_cin();
			std::cin.get();
			continue;
		}
		else if (test_num != 15)
		{
			serv = new server(port, pass);
			serv->init_pollfd();
		}
		switch (test_num)
		{
			case 2:
				test_add_user(serv, 4, "63.161.169.138", 3490);
				test_add_user(serv, 5, "63.161.169.137", 3491);
				test_add_user(serv, 6, "63.161.169.138", 3491);
				serv->delete_user(3);
				serv->delete_user(2);
				serv->delete_user(1);
				break;
			case 3:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				serv->add_user(5, build_address("63.161.169.137", 3491));
				test_delete_user(serv, 2);
				serv->delete_user(1);
				break;
			case 4:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				test_getters(serv);
				serv->delete_user(1);
				break;
			case 5:
				test_parse_message(serv, "USER paco");
				break;
			case 6:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				serv->add_user(5, build_address("63.161.169.137", 3491));
				test_user_cmd(serv); 
				serv->delete_user(2);
				serv->delete_user(1);
				break;
			case 7:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				serv->add_user(5, build_address("63.161.169.137", 3491));
				test_nick_cmd(serv);
				serv->delete_user(2);
				serv->delete_user(1);
				break;
			case 8:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				serv->add_user(5, build_address("63.161.169.137", 3491));
				test_join_cmd(serv);
				serv->delete_user(2);
				serv->delete_user(1);
				break;
			// fd_ready? execute_command?
			case 9:
			// pong
				break;
			case 10:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				test_quit_cmd(serv);
				break;
			case 11:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				serv->add_user(5, build_address("63.161.169.137", 3491));
				test_privmsg_cmd(serv);
				serv->delete_user(2);
				serv->delete_user(1);
				break;
			case 12:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				test_topic_cmd(serv);
				serv->delete_user(1);
				break;
			case 13:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				serv->add_user(5, build_address("63.161.169.137", 3491));
				test_invite_cmd(serv);
				serv->delete_user(2);
				serv->delete_user(1);
				break;
			case 14:
				serv->add_user(4, build_address("63.161.169.138", 3490));
				serv->add_user(5, build_address("63.161.169.137", 3491));
				serv->add_user(6, build_address("63.161.169.1378", 3491));
				test_kick_cmd(serv);
				serv->delete_user(3);
				serv->delete_user(2);
				serv->delete_user(1);
				break;
			case 15:
				std::cout << MAG << "See you next time!\n";
				return 0;
		}
		delete serv;
		test_num = -1;
		std::cout << GREEN << "\nContinue...\n";
		reset_cin();
		std::cin.get();
	}
	return (0);
}
