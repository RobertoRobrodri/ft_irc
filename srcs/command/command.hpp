#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include <iostream>
#include "../server/server.hpp"
#include "../user/user.hpp"

class user;
namespace cmd
{
	void nick(int fd, user &usr, std::string name);
	void username(int fd, user &usr, std::string name);
}

#endif