#include "server/server.hpp"

int exit_msg(std::string msg)
{
	std::cout << msg << std::endl;
	return (0);
}

int main(int argc, char **argv)
{
	server *serv;
	if (argc != 4)
		return (exit_msg("Unexact number of argvs"));
	try {
		if (check_data_correct(argv) == 0)
			throw std::runtime_error("Data incorrect");
		serv = new server(argv[1], argv[2], argv[3]);
		if (serv->wait_for_connection())
			throw std::runtime_error("Conection failed");
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
	return (0);
}
