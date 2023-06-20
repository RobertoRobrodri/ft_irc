#include "command.hpp"
#include "reply.hpp"

// NICKNAME

static bool invalid_nick(std::string str)
{
  if (str.find(" ") || str.find(",") || str.find("?") || str.find("!") || str.find("@") || str.find("*")
    || (str[0] == ':') || (str[0] == '$') || (str.length() > 9))
    return true;
  return false;
}

// TODO https://modern.ircdocs.horse/#errerroneusnickname-432
void cmd::nick(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  //get reference of the user
  user &usr = svr.get_user(pollfd.fd);
  std::vector <std::string> splitted = ft_split(str, ' ');
  //set nick
  if (str == "NICK")
  {
    std::string send_msg_to_user = ": 431: No nickname was given \r\n";
	  svr.send_message(const_cast<char *>(send_msg_to_user.c_str()), usr.get_fd(), send_msg_to_user.length());
	  return ;
  }
  if ((splitted.size() > 1) || invalid_nick(std::string str))
  {
    std::string send_msg_to_user = ": 432 " + str + " : Erroneus nickname \r\n";
	  svr.send_message(const_cast<char *>(send_msg_to_user.c_str()), usr.get_fd(), send_msg_to_user.length());
	  return ;
  }
  if (svr.get_user_from_nick(str) != 0)
  {
    std::string send_msg_to_user = ": 433 " + str + " : Nickname is already in use \r\n";
	  svr.send_message(const_cast<char *>(send_msg_to_user.c_str()), usr.get_fd(), send_msg_to_user.length());
	  return ;
  }
  usr.set_nick(str);
  usr.is_registered(svr);
  std::cout << usr << std::endl;
}

//  The USER message is used at the beginning of connection to specify
//    the username, hostname, servername and realname of s new user.
void  cmd::username(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);

  // Separar el resto del realname
  std::vector<std::string> cmd_params = ft_split(str, ':');
  std::vector<std::string> other_params = ft_split(cmd_params[0], ' ');
  usr.set_username(other_params[0]);
  usr.set_hostname(other_params[1]);
  usr.set_servername(other_params[2]);
  usr.set_realname(cmd_params[1]);

  // IF registered --> Send RPL_WELCOME
  //TODO meterlo en el define
  usr.is_registered(svr);
  std::cout << usr << std::endl;
}

void  cmd::pong(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
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