#include "command.hpp"

// NICKNAME

void cmd::nick(server &svr, int poll_fd_pos, std::string name) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  //get reference of the user
  user &usr = svr.get_user(pollfd.fd);
  //set nick
  usr.set_nick(name);
  std::cout << usr << std::endl;
}

void  cmd::username(server &svr, int poll_fd_pos, std::string name) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  usr.set_username(name);
  std::cout << usr << std::endl;
}

void  cmd::quit(server &svr, int poll_fd_pos, std::string name) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  svr.send_message(const_cast<char *>(name.c_str()), usr.get_fd(), name.length());
  svr.delete_user(poll_fd_pos);
}
