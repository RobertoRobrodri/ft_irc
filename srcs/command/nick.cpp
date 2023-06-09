#include "command.hpp"
#include "reply.hpp"

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
