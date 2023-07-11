#include "command.hpp"
#include "reply.hpp"

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