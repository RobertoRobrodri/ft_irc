#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include <iostream>
#include "../server/server.hpp"
#include "../user/user.hpp"

class user;
class server;
namespace cmd
{
	void nick(server &svr, int poll_fd_pos, std::string str);
	void username(server &svr, int poll_fd_pos, std::string str);
	void pong(server &svr, int poll_fd_pos, std::string str);
	void quit(server &svr, int poll_fd_pos, std::string str);
	void privmsg(server &svr, int poll_fd_pos, std::string str);
	void join(server &svr, int poll_fd_pos, std::string str);
	void topic(server &svr, int poll_fd_pos, std::string str);
	void invite(server &svr, int poll_fd_pos, std::string str);
	void kick(server &svr, int poll_fd_pos, std::string str);
	void notice(server &svr, int poll_fd_pos, std::string str);
	void part(server &svr, int poll_fd_pos, std::string str);
	void mode(server &svr, int poll_fd_pos, std::string str);
}

// TESTS
void	test_user_cmd(server *server);
void	test_nick_cmd(server *server);
void	test_join_cmd(server *server);
void	test_quit_cmd(server *server);

#endif
