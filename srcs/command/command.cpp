#include "command.hpp"

// NICKNAME

void cmd::nick(int fd, user &usr, std::string name) {
	(void)fd;
  usr.set_nick(name);
  std::cout << usr << std::endl;
}  

void  cmd::username(int fd, user &usr, std::string name) {
  (void)fd;
  usr.set_username(name);
  std::cout << usr << std::endl;
}