#include "command.hpp"

// NICKNAME

void cmd::nick(server &svr, user &usr, std::string name) {
  usr.set_nick(name);
  std::cout << usr << std::endl;
}

void  cmd::username(server &svr, user &usr, std::string name) {
  usr.set_username(name);
  std::cout << usr << std::endl;
}

void  cmd::quit(server &svr, user &usr, std::string name) {
  std::cout << usr.get_fd() << std::endl;
  svr.send_message(const_cast<char *>(name.c_str()), usr.get_fd(), name.length());
  svr.delete_user(usr.get_fd());
}
