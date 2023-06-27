#include "../srcs/server/server.hpp"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
	(void)argc;

	std::cout << "Test check_data_correct\n";
	std::cout << "==========================\n";
	
	argv = (char *[]){"./ircserv", "", "", ""};
	std::cout << std::boolalpha;
	std::cout << "Params: \"\" \"\" \"\" \"\" \n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";
	
	argv = (char *[]){"./ircserv", "a", "b", "c"};
	std::cout << "Params: a b c\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";
	
	argv = (char *[]){"./ircserv", "0:0:a", "0", "a"};
	std::cout << "Params: 0:0:a 0 a\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv = (char *[]){"./ircserv", "0.0.0.:6776:pass", "6776", "pass"};
	std::cout << "Params: 0.0.0.:6776:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv = (char *[]){"./ircserv", "127.0.-1.1:6776:pass", "6776", "pass"};
	std::cout << "Params: 127.0.-1.1:6776:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv = (char *[]){"./ircserv", "127.0.0.1:-9:pass", "6776", "pass"};
	std::cout << "Params: 127.0.0.1:-9:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv = (char *[]){"./ircserv", "127.0.0.1::", "6776", "pass"};
	std::cout << "Params: 127.0.0.1:: 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv = (char *[]){"./ircserv", "127.0.0.1", "6776", "pass"};
	std::cout << "Params: 127.0.0.1 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	std::cout << "Test server construction\n";
	std::cout << "==========================\n";
	server *serv;

	serv = new server("", "", "");
	std::cout << "Params: \n";
	std::cout << "Server: \n" << serv << "\n";


	if (serv->wait_for_connection())
		throw std::runtime_error("Conection failed");
	return (0);
}
