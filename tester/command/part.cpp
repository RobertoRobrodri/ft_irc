#include "command.hpp"
#include "reply.hpp"

void  cmd::part(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  if (usr.get_is_registered() == true)
  {
    if (str == "PART")
    {
      svr.send_message(": 461: PART: Not enough parameters \r\n", usr.get_fd());
      return ;
    }
    std::vector<std::string> chnlist = ft_split(str, ',');
    for (size_t i = 0; i < chnlist.size(); i++)
    {
      channel *chn = svr.get_channel_from_name(chnlist[i]);
      if (!chn)
      {
          svr.send_message(": 403: " + chnlist[i] + " No such channel \r\n", usr.get_fd());
          continue;
      }
      if (!chn->is_user_in_channel(usr))
      {
        svr.send_message(": 442: " + chnlist[i] + " You're not on that channel \r\n", usr.get_fd());
        continue ;
      }
      chn->rmv_member(usr);
    }
  }
}