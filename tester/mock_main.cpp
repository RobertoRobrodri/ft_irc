#include "server/server.hpp"
#include "command/command.hpp"
#include <iostream>
#include <iomanip>
#include <unistd.h>

int main()
{
	test_check_data_correct();
	
	char *arg2 = "6776";
	char *arg3 = "pass";
	server *serv = test_server_construction(arg2, arg3);
	test_connection(serv);

	return (0);
}
