#include "command.hpp"
#include "reply.hpp"

void  cmd::part(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  if (str == "PART")
  {
    std::string cmd = "PART";
	  svr.send_message(ERR_NEEDMOREPARAMS(cmd), usr.get_fd());
	  return ;
  }
  std::vector<std::string> chnlist = ft_split(str, ',');
  for (size_t i = 0; i < chnlist.size(); i++)
  {
    channel *chn = svr.get_channel_from_name(chnlist[i]);
    if (!chn)
    {
        svr.send_message(ERR_NOSUCHCHANNEL, usr.get_fd());
        continue;
    }
    if (!chn->is_user_in_channel(usr))
    {
      svr.send_message(ERR_NOTONCHANNEL(chnlist[i]), usr.get_fd());
      continue ;
    }
    chn->rmv_member(usr);
  }
}
// TODO ? Debería mandar mensaje fulano salió del canal?
