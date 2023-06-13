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
	void nick(server &svr, user &usr, std::string name);
	void username(server &svr, user &usr, std::string name);
	void quit(server &svr, user &usr, std::string name);
}

#endif