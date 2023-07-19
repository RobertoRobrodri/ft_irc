#include "command.hpp"
#include "reply.hpp"

void cmd::mode(server &svr, int poll_fd_pos, std::string str)
{
	poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  	user &usr = svr.get_user(pollfd.fd);
  	std::vector<std::string> msglist = ft_split(str, ' ');
	// ERR_NEEDMOREPARAMS
  	if (msglist.size() < 2)
  	{
  	  svr.send_message(": 461 MODE: Not enough parameters \r\n", usr.get_fd());
  	  return;
  	}
	channel *chn = svr.get_channel_from_name(msglist[0]);
	// ERR_NOSUCHCHANNEL
	if (chn == NULL)
	{
		svr.send_message(": 403 " + msglist[0] + ": No such channel \r\n", usr.get_fd());
		return;
	}
	//  ERR_NOTONCHANNEL
	if (chn->is_user_in_channel(usr) == false)
	{
		svr.send_message(": 442 " + msglist[0] + ": You're not on that channel \r\n", usr.get_fd());
		return;
	}
	// ERR_CHANOPRIVSNEEDED
	if (chn->is_user_operator(usr) == false)
	{
		svr.send_message(": 482 " + msglist[0] + ":You're not channel operator \r\n", usr.get_fd());
		return;
	}
	std::vector<std::string> mode_params = msglist;
	mode_params.erase(mode_params.begin(), mode_params.begin() + 2);
	chn->parse_mode_flag(msglist[1], mode_params, svr);
	std::cout << GREEN << *chn << RESET << std::endl;
}