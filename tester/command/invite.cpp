#include "command.hpp"
#include "reply.hpp"

/*
 Parameters: <nickname> <channel>

   The INVITE message is used to invite users to a channel.
   <nickname> is the nickname of the person to be invited to <channel>.
   There is no requirement that the channel must exist or be valid.
   To invite a user to a channel which is invite only (MODE +i), the 
   client sending the invite must be a channel operator on that  channel.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_USERONCHANNEL
           ERR_CHANOPRIVSNEEDED
           RPL_INVITING                    RPL_AWAY

   Examples:

   :Angel INVITE Wiz #Dust         ; User Angel inviting WiZ to channel
                                   #Dust

   INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to
                                   #Twilight_zone
 */

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

void	test_invite_cmd(server *server)
{
	std::cout << BLUE << "Test invite command\n";
	std::cout << "==========================\n" << RESET;

	cmd::join(*server, 1, "#TestChannel");
    channel *channel1 = server->get_channel_from_name("#TestChannel");
	cmd::nick(*server, 2, "Wiz");
	
	std::cout <<  CYAN << "Test 1: Invite to an existing channel\n" << RESET;
	std::cout << YELLOW << "INVITE Wiz #TestChannel\n" << RESET;
	cmd::topic(*server, 1, "Wiz #TestChannel");
	
	std::cout <<  CYAN << "Test 2: Invite to a non existing channel\n" << RESET;
	std::cout << YELLOW << "INVITE Wiz #NoExist\n" << RESET;
	cmd::topic(*server, 1, "Wiz #NoExist");
	
	std::cout <<  CYAN << "Test 3: No channel\n" << RESET;
	std::cout << YELLOW << "INVITE Wiz\n" << RESET;
	cmd::topic(*server, 1, "Wiz");
	
	std::cout <<  CYAN << "Test 3: No parameters\n" << RESET;
	std::cout << YELLOW << "INVITE\n" << RESET;
	cmd::topic(*server, 1, "");
}
