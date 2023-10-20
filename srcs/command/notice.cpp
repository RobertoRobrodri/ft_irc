/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:17:00 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/20 11:50:59 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "reply.hpp"

void  cmd::notice(server &svr, int poll_fd_pos, std::string str)
{
	poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
	user &usr = svr.get_user(pollfd.fd);
	if (usr.get_is_registered() == true)
	{
		if (str == "")
			return ;
		std::vector<std::string> msglist = ft_split(str, ' ');
		if (msglist.size() == 1)
			return ;
		std::vector<std::string> rcvlist = ft_split(msglist[0], ',');
		std::string msg = str.substr(str.find(msglist[1]));
		std::sort(rcvlist.begin(), rcvlist.end());
		for (size_t i = 0; i < rcvlist.size(); i++)
		{
			if (i == 0 || rcvlist[i] != rcvlist[i - 1])
			{
				if ((rcvlist[i][0] == '#') || (rcvlist[i][0] == '&'))
				{
					channel *chn = svr.get_channel_from_name(rcvlist[i]);
					usr.send_to_channel(svr, chn, rcvlist[i], msg);
				}
				else
				{
					user *receiver = svr.get_user_from_nick(rcvlist[i]);
					if (receiver)
						svr.send_message("From " + usr.get_nick() + ":\n" + msg + "\n", receiver->get_fd());
				}
			}
		}
  	}
}
