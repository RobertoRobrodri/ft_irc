#include "command.hpp"
#include "reply.hpp"

void  cmd::notice(server &svr, int poll_fd_pos, std::string str) {
	std::string command = "NOTICE";
	poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  if (usr.get_is_registered() == true)
  {

    if (str == "")
    {
      svr.send_message(ERR_NORECIPIENT(command), usr.get_fd());
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
    for (size_t i = 0; i < rcvlist.size(); i++)
    {
      if ((rcvlist[i][0] == '#') || (rcvlist[i][0] == '&'))
      {
        channel *chn = svr.get_channel_from_name(rcvlist[i]);
        usr.send_to_channel(svr, chn, rcvlist[i], msg);
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
}
