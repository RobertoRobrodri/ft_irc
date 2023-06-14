#include "command.hpp"

// NICKNAME

void cmd::nick(server &svr, int fd, std::string name) {
  //get reference of the user
  user &usr = svr.get_user(fd);
  //set nick
  usr.set_nick(name);
  std::cout << usr << std::endl;
}

void  cmd::username(server &svr, int fd, std::string name) {
  user &usr = svr.get_user(fd);
  usr.set_username(name);
  std::cout << usr << std::endl;
}

void  cmd::quit(server &svr, int fd, std::string name) {
  user &usr = svr.get_user(fd);
  svr.send_message(const_cast<char *>(name.c_str()), usr.get_fd(), name.length());
  svr.delete_user(fd);
}
