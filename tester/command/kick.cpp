#include "command.hpp"
#include "reply.hpp"

/*
  Parameters: <channel> <user> [<comment>]

    forcibly  remove  a  user  from  a channel
    Only a channel operator may kick another user out of a  channel.
 	It is possible to extend the KICK command parameters to:
	<channel>{,<channel>} <user>{,<user>} [<comment>]
   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
           ERR_NOTONCHANNEL
 */

void cmd::kick(server &svr, int poll_fd_pos, std::string str)
{
	std::string command = "KICK";
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  std::vector<std::string> msglist = ft_split(str, ' ');
  if (msglist.size() < 2)
  {
    svr.send_message(ERR_NEEDMOREPARAMS(command), usr.get_fd());
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
      svr.send_message(ERR_NOSUCHNICK(chnlist[i]), usr.get_fd());
    else
    {
      if (!chn->is_user_in_channel(usr))
      {
        svr.send_message(ERR_NOTONCHANNEL(chnlist[i]), usr.get_fd());
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

void	test_kick_cmd(server *server)
{
	std::cout << BLUE << "Test kick command\n";
	std::cout << "==========================\n" << RESET;

	cmd::username(*server, 1, "Admin host server administrator");
	cmd::username(*server, 2, "Anon host server anonymous");
	cmd::username(*server, 3, "MrTipsy host server tipsyman");
	cmd::join(*server, 1, "#TestChannel");
	cmd::join(*server, 2, "#TestChannel");
    channel *channel1 = server->get_channel_from_name("#TestChannel");
	
	std::cout <<  CYAN << "Test 1: Kick other user\n" << RESET;
	std::cout << YELLOW << "KICK #TestChannel Anon\n" << RESET;
	cmd::kick(*server, 1, "#TestChannel Anon");
	std::cout << *channel1 << std::endl;
	
	std::cout <<  CYAN << "Test 2: Kick non-existing user\n" << RESET;
	std::cout << YELLOW << "KICK #TestChannel hsdgjhagjfh\n" << RESET;
	cmd::kick(*server, 1, "#TestChannel hsdgjhagjfh");
	
	std::cout <<  CYAN << "Test 3: Kick from non-existing channel\n" << RESET;
	std::cout << YELLOW << "KICK gievrhiugw MrTipsy\n" << RESET;
	cmd::kick(*server, 1, "gievrhiugw MrTipsy");
	
	std::cout <<  CYAN << "Test 4: Not enough params\n" << RESET;
	std::cout << YELLOW << "KICK\n" << RESET;
	cmd::kick(*server, 1, "");
	
	std::cout <<  CYAN << "Test 5: Kick oneself\n" << RESET;
	std::cout << YELLOW << "KICK #TestChannel Admin\n" << RESET;
	cmd::kick(*server, 1, "#TestChannel Admin");
	std::cout << *channel1 << std::endl;
}
