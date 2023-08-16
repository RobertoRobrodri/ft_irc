#include "server/server.hpp"
#include "command/command.hpp"
#include <iostream>
#include <iomanip>

int main()
{
	test_check_data_correct();
	
	char *arg2 = "6776";
	char *arg3 = "pass";

	server *serv = test_server_construction(arg1, arg2, arg3);
	
	// autosocket
	
	// init_socket_struct
	
	// server_listening
	
	//UNCOMMENT TO TEST CONNECTION (this will block the program)
	//test_connection(serv);
	
	serv->init_pollfd();
	
	test_add_user(serv, 3, "63.161.169.137", 3490);
	test_add_user(serv, 4, "63.161.169.138", 3490);
	test_add_user(serv, 5, "63.161.169.137", 3491);
	test_delete_user(serv, 2);
	test_getters(serv);
	// fd_ready
	test_parse_message(serv, "USER paco");
	
	// execute_command 

	cmd_map commands = serv->list_of_cmds;

	// user
	test_user_cmd(*serv); 

	// nick
	test_nick_cmd(*serv);
	std::cout << std::endl;

	// join
	test_join_cmd(*serv);
	std::cout << std::endl;

	// pong

	// quit

	// privmsg

	// topic

	// invite

	// kick	

	return (0);
}
