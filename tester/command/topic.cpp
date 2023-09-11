#include "command.hpp"
#include "reply.hpp"

/*
  Parameters: <channel> [<topic>]

   The TOPIC message is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic> given.
   If the <topic> parameter is present, the topic for that channel will be
   changed if the channel modes permit this action.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
           RPL_NOTOPIC                     RPL_TOPIC
           ERR_CHANOPRIVSNEEDED

 */
void cmd::topic(server &svr, int poll_fd_pos, std::string str) //TODO los modos
{
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  if (str == "")
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

void	test_topic_cmd(server *server)
{
	std::cout << BLUE << "Test topic command\n";
	std::cout << "==========================\n" << RESET;

	cmd::join(*server, 1, "#TestChannel");
    channel *channel1 = server->get_channel_from_name("#TestChannel");

	std::cout << CYAN << "Test 1: Set a topic\n" << RESET;
	std::cout << YELLOW << "TOPIC #TestChannel :New topic\n" << RESET;
	cmd::topic(*server, 1, "#TestChannel :New topic");
	std::cout << *channel1 << std::endl;

	std::cout << CYAN << "Test 2: Change the topic\n" << RESET;
	std::cout << YELLOW << "TOPIC #TestChannel :Other topic\n" << RESET;
	cmd::topic(*server, 1, "#TestChannel :Other topic");
	std::cout << *channel1 << std::endl;

	// Check if the server sends RPL_TOPIC
	std::cout << CYAN << "Test 3: Check a channel's topic\n" << RESET;
	std::cout << YELLOW << "TOPIC #TestChannel\n" << RESET;
	cmd::topic(*server, 1, "#TestChannel");
	
	std::cout << CYAN << "\nTest 4: No parameters\n" << RESET;
	std::cout << YELLOW << "TOPIC\n" << RESET;
	cmd::topic(*server, 1, "");
}
