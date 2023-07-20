#include "server/server.hpp"
#include <iostream>
#include <iomanip>

int main()
{
	test_check_data_correct();
	
	char *arg2 = "6776";
	char *arg3 = "pass";
	server *serv = test_server_construction(arg2, arg3);
	serv->init_pollfd();
	test_add_user(serv, 4, "63.161.169.137", 3490);
	test_add_user(serv, 5, "63.161.169.138", 3490);
	test_delete_user(serv, 1);
	test_parse_message(serv, "USER paco");
	//UNCOMMENT TO TEST CONNECTION (this will block the program)
	test_connection(serv);
	
	return (0);
}
