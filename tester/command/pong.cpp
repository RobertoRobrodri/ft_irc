#include "command.hpp"
#include "reply.hpp"

void  cmd::pong(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  //svr.send_message(const_cast<char *>(str.c_str()), usr.get_fd(), str.length());
}