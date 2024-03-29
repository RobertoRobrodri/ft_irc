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
void cmd::topic(server &svr, int poll_fd_pos, std::string str)
{
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);

  if (usr.get_is_registered() == true)
  {
	  std::string command = "TOPIC";
    if (str == "")
    {
      svr.send_message(ERR_NEEDMOREPARAMS(command), usr.get_fd());
      return ;
    }
    std::vector<std::string> msglist = ft_split(str, ' ');
    channel *chn = svr.get_channel_from_name(msglist[0]);
    if (chn)
    {
      if (chn->is_user_in_channel(usr))
      {
		    if (chn->get_mode().find("t") != std::string::npos && !usr.get_op())
        {
            svr.send_message(ERR_CHANOPRIVSNEEDED(msglist[0]), usr.get_fd());
            return ;
        }
        if (msglist.size() == 1)
        {
          std::string topic = chn->get_topic();
          if (topic.empty())
          {
            svr.send_message(RPL_NOTOPIC(msglist[0]), usr.get_fd());
            return ;
          }
          svr.send_message(RPL_TOPIC(msglist[0], topic), usr.get_fd());
        }
        else
        {
          if (chn->get_user_from_nick(usr.get_nick())->get_op() == false)
          {
            svr.send_message(ERR_CHANOPRIVSNEEDED(msglist[0]), usr.get_fd());
            return ;
          }
          std::string new_topic = str.substr(str.find(msglist[1]));
          chn->set_topic(str.substr(str.find(msglist[1])));
          return ;
        }

      }
      else
      {
        svr.send_message(ERR_NOTONCHANNEL(msglist[0]), usr.get_fd());
        return ;
      }
    }
    else
    {
      svr.send_message(ERR_NOSUCHCHANNEL(msglist[0]), usr.get_fd());
      return ;
    }
  }
  else
	  svr.send_message(ERR_NOTREGISTERED, usr.get_fd());
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
