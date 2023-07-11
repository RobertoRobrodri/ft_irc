#include "command.hpp"
#include "reply.hpp"

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