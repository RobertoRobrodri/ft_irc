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
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  if (str == "PRIVMSG")
  {
	  svr.send_message(": 411: No recipient given (PRIVMSG) \r\n", usr.get_fd());
	  return ;
  }
  std::vector<std::string> msglist = ft_split(str, ' ');
  if (msglist.size() == 1)
  {
    svr.send_message(": 412: No text to send \r\n", usr.get_fd());
	  return ;
  }
  std::vector<std::string> rcvlist = ft_split(msglist[0], ',');
	std::string msg = str.substr(str.find(msglist[1]));
  for (int i = 0; i < rcvlist.size(); i++)
  {
    if ((rcvlist[i][0] == '#') || (rcvlist[i][0] == '&'))
    {
      channel *chn = svr.get_channel_from_name(rcvlist[i]);
      usr.send_to_channel(chn, svr, rcvlist[i], msg);
    }
    else
    {
      user *receiver = svr.get_user_from_nick(rcvlist[i]);
      if(receiver)
        svr.send_message("From " + usr.get_nick() + ":\n" + msg + "\n", receiver->get_fd());
      else
        svr.send_message(": 401 " + rcvlist[i] + ": No such nick/channel \r\n", usr.get_fd());
    }
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

void cmd::topic(server &svr, int poll_fd_pos, std::string str) //TODO los modos
{
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  if (str == "TOPIC")
  {
    svr.send_message(": 461 TOPIC: Not enough parameters \r\n", usr.get_fd());
    return ;
  }
  std::vector<std::string> msglist = ft_split(str, ' ');
  channel *chn = svr.get_channel_from_name(msglist[0]);
  if (chn)
  {
	  if (chn->is_user_in_channel(usr))
    {
      if (msglist.size() == 1)
      {
        std::string topic = chn->get_topic();
        if (topic.empty())
        {
          svr.send_message(": 331 " + msglist[0] + ": No topic is set \r\n", usr.get_fd());
          return ;
        }
        svr.send_message(": 332 " + msglist[0] + ": " + topic + "\r\n", usr.get_fd());
      }
      else
      {
        std::string new_topic = str.substr(str.find(msglist[1]));
        chn->set_topic(str.substr(str.find(msglist[1])));
        return ;
      }

    }
    else
    {
      svr.send_message(": 442 " + msglist[0] + ": You're not on that channel \r\n", usr.get_fd());
      return ;
    }
  }
  else
  {
    svr.send_message(": 403 " + msglist[0] + ": No such channel\r\n", usr.get_fd());
    return ;
  }
}

void cmd::invite(server &svr, int poll_fd_pos, std::string str)
{
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  std::vector<std::string> msglist = ft_split(str, ' ');
  if (msglist.size() < 2)
  {
    svr.send_message(": 461 INVITE: Not enough parameters \r\n", usr.get_fd());
    return ;
  }
  user *new_user = svr.get_user_from_nick(msglist[0]);
  channel *chn = svr.get_channel_from_name(msglist[1]);
  if (!new_user)
  {
    svr.send_message(": 401 " + msglist[0] + ": No such nick/channel \r\n", usr.get_fd());
    return ;
  }
  if (!chn)
  {
    svr.send_message(": 401 " + msglist[1] + ": No such nick/channel \r\n", usr.get_fd());
    return ;
  }
  if (!chn->is_user_in_channel(usr))
  {
      svr.send_message(": 442 " + msglist[1] + ": You're not on that channel \r\n", usr.get_fd());
      return ;
  }
  if (chn->is_user_in_channel(*new_user))
  {
      svr.send_message(": 443 " + msglist[0] + " " + msglist[1] + ": is already on channel \r\n", usr.get_fd());
      return ;
  }
  chn->add_member(*new_user);
  svr.send_message(": 341 " + msglist[0] + " " + msglist[1] + " \r\n", usr.get_fd());
}

void cmd::kick(server &svr, int poll_fd_pos, std::string str)
{
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  std::vector<std::string> msglist = ft_split(str, ' ');
  if (msglist.size() < 2)
  {
    svr.send_message(": 461 KICK: Not enough parameters \r\n", usr.get_fd());
    return ;
  }
  std::vector<std::string> chnlist = ft_split(msglist[0], ',');
  std::vector<std::string> usrlist = ft_split(msglist[1], ',');
  if (msglist.size() > 2)
	  std::string msg = str.substr(str.find(msglist[2]));
  for (int i = 0; i < chnlist.size(); i++)
  {
    channel *chn = svr.get_channel_from_name(chnlist[i]);
    if (!chn)
      svr.send_message(": 401 " + chnlist[i] + ": No such nick/channel \r\n", usr.get_fd());
    else
    {
      if (!chn->is_user_in_channel(usr))
      {
        svr.send_message(": 442 " + chnlist[i] + ": You're not on that channel \r\n", usr.get_fd());
        continue ;
      }
      for (int j = 0; j < usrlist.size(); j++)
      {
        user *rcv = svr.get_user_from_nick(usrlist[j]);
        if (rcv)
          chn->rmv_member(*rcv);
      }
    }
  }
}