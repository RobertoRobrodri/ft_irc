#include "command.hpp"

// NICKNAME

void cmd::nick(server &svr, int fd, std::string name) {

  svr.get_user(fd).set_nick(name);
  std::cout << svr.get_user(fd) << std::endl;
}

void  cmd::username(server &svr, int fd, std::string name) {
  svr.get_user(fd).set_username(name);
  std::cout << svr.get_user(fd) << std::endl;
}

void  cmd::quit(server &svr, int fd, std::string name) {
  svr.send_message(const_cast<char *>(name.c_str()), svr.get_user(fd).get_fd(), name.length());
  svr.delete_user(fd);
}
