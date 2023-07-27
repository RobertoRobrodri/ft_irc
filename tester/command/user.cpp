#include "command.hpp"
#include "reply.hpp"

//  The USER message is used at the beginning of connection to specify
//    the username, hostname, servername and realname of a new user.
//    Ref: https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.3
//
//    mode parameter should be numeric: a 2 bit bitmask
//    	if the bit 2 is set, the user mode 'w' will be set
//    	if the bit 3 is set, the user mode 'i' will be set
//
//	  realname may contain spaces
//
//	  ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
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

void	test_user_cmd()
{
}
