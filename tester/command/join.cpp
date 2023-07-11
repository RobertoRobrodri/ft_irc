#include "command.hpp"
#include "reply.hpp"

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
	  svr.send_message(": 461 JOIN :Not enough parameters \r\n", usr.get_fd());
    return ;
  }
  // Me pueden pasar una serie de canales separados por ','
  // TODO Channel_params[1] debe contener las contraseñas, si se utilizan
  std::vector<std::string> channels_to_join = ft_split(channel_params[0], ',');
  std::vector<std::string> passwords = ft_split(channel_params[1], ',');
  std::vector<std::string>::iterator password_it = passwords.begin();
  for (std::vector<std::string>::iterator it = channels_to_join.begin(); it != channels_to_join.end(); it++)
  {
    channel *chn = svr.get_channel_from_name(*it);
    if (chn)
    {
      if (chn->is_user_in_channel(usr) == true)
      {
          svr.send_message(": 443 " + usr.get_nick() + " " + chn->get_name() + ": is already on channel \r\n", usr.get_fd());
          continue ;
      }
      else
      {
        // Invite only channel
        if (chn->get_mode().find("i") != std::string::npos)
        {
          svr.send_message(": 473 " + chn->get_name() + ":Cannot join channel (+i \r\n)", usr.get_fd());
          continue ;
        }
        // Requires password
        if (chn->get_mode().find("k") != std::string::npos)
        {
          if (chn->get_password().compare(*password_it))
          {
            svr.send_message(": 475 " + chn->get_name() + ":Cannot join channel (+k) \r\n", usr.get_fd());
            continue ;
          }
          password_it++;
        }
        if (chn->get_mode().find("l") != std::string::npos)
        {
          if (chn->get_list_of_members().size() >= chn->get_user_limit())
          {
            svr.send_message(": 471 " + chn->get_name() + ":Cannot join channel (+l) \r\n", usr.get_fd());
            continue ;
          }
        }
        if (usr.get_n_channels() >= MAX_NUMBER_OF_CHN)
        {
            svr.send_message(": 405 " + chn->get_name() + ":You have joined too many channels\r\n" , usr.get_fd());
            return ;
        }
        chn->add_member(usr);
      }
    }
    else
    {
      if (usr.get_n_channels() >= MAX_NUMBER_OF_CHN)
      {
            svr.send_message(": 405 " + *it + ":You have joined too many channels\r\n" , usr.get_fd());
            return ;
      }
      svr.create_channel(usr, *it);
    }
  }
}