#include "command.hpp"

// NICKNAME

// TODO https://modern.ircdocs.horse/#errerroneusnickname-432
void cmd::nick(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  //get reference of the user
  user &usr = svr.get_user(pollfd.fd);
  //set nick
  // if (svr.compare_nicknames(name) == 0)
  // {
	// name.append(":Nickname is already in use");
	// svr.send_message(const_cast<char *>(name.c_str()), usr.get_fd(), name.length());
	// return ;
  // }
  usr.set_nick(str);
  std::cout << usr << std::endl;
}

//  The USER message is used at the beginning of connection to specify
//    the username, hostname, servername and realname of s new user.
void  cmd::username(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  usr.set_username(str);
  std::cout << usr << std::endl;
}

void  cmd::pong(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  if (str.compare(TOKEN) != 0)
    return ;
  svr.send_message(const_cast<char *>(str.c_str()), usr.get_fd(), str.length());
}

void  cmd::quit(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  svr.send_message(const_cast<char *>(str.c_str()), usr.get_fd(), str.length());
  svr.delete_user(poll_fd_pos);
}

void  cmd::privmsg(server &svr, int poll_fd_pos, std::string str) {
  std::vector<std::string> seglist = ft_split(str, ' ');
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  user *receiver = svr.get_user_from_nick(seglist[0]);
  if(receiver)
  {
    svr.send_message(const_cast<char *>("From "), receiver->get_fd(), 1);
    svr.send_message(const_cast<char *>(usr.get_nick().c_str()), receiver->get_fd(), usr.get_nick().length());
    svr.send_message(const_cast<char *>(":\n"), receiver->get_fd(), 1);
    svr.send_message(const_cast<char *>(seglist[1].c_str()), receiver->get_fd(), seglist[1].length());
    svr.send_message(const_cast<char *>("\n"), receiver->get_fd(), 1);
  }
}

void  cmd::join(server &svr, int poll_fd_pos, std::string str) {
  // Si no existe canal:
  // SVR->crear_canal()
  // else
  // SVR->add_user_to_channel()
  std::vector<std::string> seglist = ft_split(str, ' ');
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  channel *cnn = svr.get_channel_from_name(seglist[0]);
  if (cnn)
    cnn->add_member(usr);
  else
    svr.create_channel(usr, seglist[0]);
}