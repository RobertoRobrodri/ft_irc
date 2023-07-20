#include "command.hpp"
#include "reply.hpp"

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
  for (size_t i = 0; i < chnlist.size(); i++)
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
      for (size_t j = 0; j < usrlist.size(); j++)
      {
        user *rcv = svr.get_user_from_nick(usrlist[j]);
        if (rcv)
          chn->rmv_member(*rcv);
      }
    }
  }
}