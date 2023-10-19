/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:14:07 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/19 17:13:27 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
 */

void cmd::invite(server &svr, int poll_fd_pos, std::string str)
{
	poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
	user &usr = svr.get_user(pollfd.fd);
	
	/* ERROR MESSAGES */
	std::string command = "INVITE";
  	std::vector<std::string> msglist = ft_split(str, ' ');
	if (str == "" || msglist.size() < 2)
		return svr.send_message(ERR_NEEDMOREPARAMS(command), usr.get_fd());
	
	if (usr.get_is_registered() == false)
		return svr.send_message(ERR_NOTREGISTERED, usr.get_fd());
  
  	user *new_user = svr.get_user_from_nick(msglist[0]);
  	if ((!new_user) || (new_user->get_is_registered() == false))
		return svr.send_message(ERR_NOSUCHNICK(msglist[0]), usr.get_fd());
	
	channel *chn = svr.get_channel_from_name(msglist[1]);
  	if (!chn)
    	return svr.send_message(ERR_NOSUCHNICK(msglist[1]), usr.get_fd());
	
	if (!chn->is_user_in_channel(usr))
		return svr.send_message(ERR_NOTONCHANNEL(msglist[1]), usr.get_fd());
	
	if (chn->get_mode().find('i') != std::string::npos && !chn->is_user_operator(usr))
		return svr.send_message(ERR_CHANOPRIVSNEEDED(msglist[1]), usr.get_fd());

	if (chn->is_user_in_channel(*new_user))
		return svr.send_message(ERR_USERONCHANNEL(msglist[0], msglist[1]), usr.get_fd());
	
	/* COMMAND ACCEPTED */
	chn->add_member(*new_user);
	svr.send_message(RPL_INVITING(msglist[0], msglist[1]), usr.get_fd());
}
