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
	void nick(server &svr, int poll_fd_pos, std::string name);
	void username(server &svr, int poll_fd_pos, std::string name);
	void pingpong(server &svr, int poll_fd_pos, std::string name);
	void quit(server &svr, int poll_fd_pos, std::string name);

	void privmsg(server &svr, int poll_fd_pos, std::string name);
}

#endif