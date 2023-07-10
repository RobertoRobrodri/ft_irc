#include "server/server.hpp"
#include <iostream>
#include <iomanip>

int main()
{
	test_check_data_correct();
	
	char *arg1 = "127.0.0.1:6776:pass";
	char *arg2 = "6776";
	char *arg3 = "pass";
	server *serv = test_server_construction(arg1, arg2, arg3);
	test_add_user(serv);
	test_delete_user(serv, 1);
	//UNCOMMENT TO TEST CONNECTION (this will block the program)
	test_connection(serv);
	
	return (0);
}
