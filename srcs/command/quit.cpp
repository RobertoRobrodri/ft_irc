/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:19:08 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/23 17:20:50 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "reply.hpp"
#include "../server/server.hpp"

void  cmd::quit(server &svr, int poll_fd_pos, std::string str)
{
	poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
	user *usr = svr.get_user(pollfd.fd);
	std::map<std::string, channel*>::iterator	it;
	std::vector<user*>::iterator				itu;
	std::vector<user*>							chn_usrs, common_usrs;
	channel* 									chn;
	
	if (str.empty())
		str = "Closing connection\r\n";
	for (it = svr.list_of_channels.begin(); it != svr.list_of_channels.end(); it++)
	{
		chn = it->second;
		if (chn->is_user_in_channel(usr))
		{
			chn_usrs = chn->get_list_of_members();
			for (itu = chn_usrs.begin(); itu != chn_usrs.end(); itu++)
			{
				if (((*itu)->get_nick() != usr->get_nick()) && (std::count(common_usrs.begin(), common_usrs.end(), *itu) == 0))
				{
					svr.send_message("From " + usr->get_nick() + ":\n" + str + "\n", (*itu)->get_fd());
					common_usrs.push_back(*itu);
				}
			}
		}
	}
	svr.delete_user(poll_fd_pos);
}
