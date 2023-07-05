#include "server/server.hpp"
#include <iostream>
#include <iomanip>

int main()
{
	char *program = "./ircserv";
	char *arg1 = "";
	char *arg2 = "";
	char *arg3 = "";

	char *argv[] = {program, arg1, arg2, arg3, NULL};

	std::cout << "Test check_data_correct\n";
	std::cout << "==========================\n";
	std::cout << std::boolalpha;

	std::cout << "Params: \" \" \" \" \" \"\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";
	
	argv[1] = "a";
	argv[2] = "b";
	argv[3] = "c";
	std::cout << "Params: a b c\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";
	
	argv[1] = "0:0:a";
	argv[2] = "0";
	argv[3] = "a";
	std::cout << "Params: 0:0:a 0 a\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "0.0.0.:6776:pass";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 0.0.0.:6776:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "127.0.-1.1:6776:pass";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 127.0.-1.1:6776:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "127.0.0.1:-9:pass";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 127.0.0.1:-9:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "127.0.0.1::";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 127.0.0.1:: 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "127.0.0.1";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 127.0.0.1 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "127.0.0.1::6776:pass";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 127.0.0.1::6776:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "127.0.0.1:6776:pass";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 127.0.0.1:6776:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	std::cout << "Test server construction\n";
	std::cout << "==========================\n";
	server *serv;

	serv = new server(argv[1], argv[2], argv[3]);
	std::cout << *serv << std::endl;

	std::map<std::string, command_function>::iterator it;

    std::cout << "Commands:" << std::endl;
	for (it = serv->list_of_cmds.begin(); it != serv->list_of_cmds.end(); it++)
	{
    	std::cout << it->first << std::endl;
	}
	return (0);
}
