#include "command.hpp"
#include "reply.hpp"
#include "../server/server.hpp"

void  cmd::quit(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  // BUG si no pasamos parámetros str == QUIT
  // Qué pasa si quiero que mi mensaje sea QUIT?
  // Este comportamiento se repite para cada comando
  if (str.empty())
	str += "Cierra al salir (Mensaje por defecto del server)";
  str += "\r\n";
  svr.send_message(str, usr.get_fd());
  svr.delete_user(poll_fd_pos);
}

void	test_quit_cmd(server *server)
{
	std::cout << BLUE << "Test quit command\n";
	std::cout << "==========================\n" << RESET;

	std::cout << CYAN << "Test 1: Quit without message\n" << RESET;
	std::cout << YELLOW << "QUIT\n" << RESET;
	print_poll_fd(server->_active_fds, server->poll_fds);
	print_list_of_users(server->list_of_users);
	cmd::quit(*server, 1, "");
	print_poll_fd(server->_active_fds, server->poll_fds);
	print_list_of_users(server->list_of_users);

	server->add_user(4, build_address("63.161.169.138", 3490));
	std::cout <<  CYAN << "\nTest 2: Quit with message\n" << RESET;
	std::cout << YELLOW << "QUIT :Gone to have lunch\n" << RESET;
	print_poll_fd(server->_active_fds, server->poll_fds);
	print_list_of_users(server->list_of_users);
	cmd::quit(*server, 1, ":Gone to have lunch");
	print_poll_fd(server->_active_fds, server->poll_fds);
	print_list_of_users(server->list_of_users);
}
