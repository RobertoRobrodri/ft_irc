#include "server/server.hpp"
#include "command/command.hpp"
#include <iostream>
#include <iomanip>
#include <unistd.h>

int main()
{
	char *port = "6667";
	char *pass = "pass";
	server *serv = new server(port, pass);
	test_connection(serv);

	return (0);
}
