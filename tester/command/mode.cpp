#include "command.hpp"
#include "reply.hpp"

void cmd::mode(server &svr, int poll_fd_pos, std::string str)
{
	std::string command = "MODE";
	poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  	user &usr = svr.get_user(pollfd.fd);
  	std::vector<std::string> msglist = ft_split(str, ' ');
  	if (msglist.size() < 2)
  	{
  	  svr.send_message(ERR_NEEDMOREPARAMS(command), usr.get_fd());
  	  return;
  	}
	channel *chn = svr.get_channel_from_name(msglist[0]);
	if (chn == NULL)
	{
		svr.send_message(ERR_NOSUCHCHANNEL(msglist[0]), usr.get_fd());
		return;
	}
	//  ERR_NOTONCHANNEL
	if (chn->is_user_in_channel(usr) == false)
	{
		svr.send_message(ERR_NOTONCHANNEL(msglist[0]), usr.get_fd());
		return;
	}
	// ERR_CHANOPRIVSNEEDED
	if (chn->is_user_operator(usr) == false)
	{
		svr.send_message(ERR_CHANOPRIVSNEEDED(msglist[0]), usr.get_fd());
		return;
	}
	std::vector<std::string> mode_params = msglist;
	mode_params.erase(mode_params.begin(), mode_params.begin() + 2);
	chn->parse_mode_flag(msglist[1], mode_params, svr);
	std::cout << GREEN << *chn << RESET << std::endl;
}
