#include "command.hpp"
#include "reply.hpp"

// NICKNAME

bool invalid_nick(std::string str)
{
  if ((str.find_first_of(" .:,?!@*&#$") != std::string::npos) || (str.length() > 9))
    return true;
  return false;
}

void cmd::nick(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  //get reference of the user
  user &usr = svr.get_user(pollfd.fd);
  //set nick
  user *nick;
  if (str == "NICK")
  {
	  svr.send_message(": 431: No nickname was given \r\n", usr.get_fd());
	  return ;
  }
  if (invalid_nick(str))
  {
	  svr.send_message(": 432 " + str + " : Erroneus nickname \r\n", usr.get_fd());
	  return ;
  }
  if ((nick = svr.get_user_from_nick(str)) != 0)
  {
    if (nick->get_nick().compare(usr.get_nick()) == 0)
      return ;
	  svr.send_message(": 433 " + str + " : Nickname is already in use \r\n", usr.get_fd());
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
  if (other_params.size() + 1 < 4)
  {
    svr.send_message(": 461 USER : <command> :Not enough parameters \r\n", usr.get_fd());
    return ;
  }
  usr.set_username(other_params[0]);
  usr.set_hostname(other_params[1]);
  usr.set_servername(other_params[2]);
  usr.set_realname(cmd_params[1]); //TODO sus

  // IF registered --> Send RPL_WELCOME
  //TODO meterlo en el define
  usr.is_registered(svr);
  std::cout << usr << std::endl;
}

void  cmd::pong(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  //svr.send_message(const_cast<char *>(str.c_str()), usr.get_fd(), str.length());
}

void  cmd::quit(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  //svr.send_message(const_cast<char *>(str.c_str()), usr.get_fd(), str.length());
  svr.delete_user(poll_fd_pos);
}

void  cmd::privmsg(server &svr, int poll_fd_pos, std::string str) {
  std::vector<std::string> seglist = ft_split(str, ' ');
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  user *receiver = svr.get_user_from_nick(seglist[0]);
  if(receiver)
  {
    // svr.send_message(const_cast<char *>("From "), receiver->get_fd(), 1);
    // svr.send_message(const_cast<char *>(usr.get_nick().c_str()), receiver->get_fd(), usr.get_nick().length());
    // svr.send_message(const_cast<char *>(":\n"), receiver->get_fd(), 1);
    // svr.send_message(const_cast<char *>(seglist[1].c_str()), receiver->get_fd(), seglist[1].length());
    // svr.send_message(const_cast<char *>("\n"), receiver->get_fd(), 1);
  }
}

void  cmd::join(server &svr, int poll_fd_pos, std::string str) {
  // Si no existe canal:
  // SVR->crear_canal()
  // else
  // SVR->add_user_to_channel()
  std::vector<std::string> channel_params = ft_split(str, ' ');
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  if (str == "JOIN")
  {
	  svr.send_message(": 461 USER : <command> :Not enough parameters \r\n", usr.get_fd());
    return ;
  }
  // Me pueden pasar una serie de canales separados por ','
  // TODO Channel_params[1] debe contener las contraseñas, si se utilizan
  std::vector<std::string> channels_to_join = ft_split(channel_params[0], ',');
  for (std::vector<std::string>::iterator it = channels_to_join.begin(); it != channels_to_join.end(); it++)
  {
    channel *cnn = svr.get_channel_from_name(*it);
    if (cnn)
      cnn->add_member(usr);
    else
      svr.create_channel(usr, *it);
  }
}