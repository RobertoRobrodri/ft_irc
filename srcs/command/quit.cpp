/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:19:08 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/23 16:09:37 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "reply.hpp"
#include "../server/server.hpp"

void  cmd::quit(server &svr, int poll_fd_pos, std::string str)
{
	poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
	user *usr = svr.get_user(pollfd.fd);
	if (str.empty())
		str = "Closing connection\r\n";

	std::map<std::string, channel*>::iterator	chan_it;
	channel* chn;
	std::vector<user*>::iterator	itu;
	std::vector<user*>	chn_usrs, common_usrs;
	for (chan_it = svr.list_of_channels.begin(); chan_it != svr.list_of_channels.end(); chan_it++)
	{
		chn = chan_it->second;
		if (chn->is_user_in_channel(usr))
			for (size_t i = 0; i < chn->get_list_of_members().size(); i++)
				if (chn->get_list_of_members()[i]->get_nick() != usr->get_nick())
					svr.send_message("From " + usr->get_nick() + ":\n" + str + "\n", \
							chn->get_list_of_members()[i]->get_fd());
	}
	//TODO gesstionar duplicados
	// for (it = svr.list_of_channels.begin(); it != svr.list_of_channels.end(); it++)
	// {
	// 	if (it->second->is_user_in_channel(usr))
	// 	{
	// 		chn_usrs = it->second->get_list_of_members();
	// 		for (itu = chn_usrs.begin(); itu != chn_usrs.end(); itu++)
	// 		{
	// 			if ((itu != usr->get_nick()) && std::count(common_usrs.begin(), common_usrs.end(), ))
	// 				common_usrs.insert(*itu);
	// 		}
	// 	}
	// }
	svr.delete_user(poll_fd_pos);
}
