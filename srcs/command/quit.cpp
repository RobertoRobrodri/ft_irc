/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:19:08 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/23 12:48:17 by crisfern         ###   ########.fr       */
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
  channel	*chn;
  for (chan_it = svr.list_of_channels.begin(); chan_it != svr.list_of_channels.end(); chan_it++)
  {
	  chn = chan_it->second;
	  if (chn->is_user_in_channel(usr))
	  {
      	for (size_t i = 0; i < chn->get_list_of_members().size(); i++)
        	if (chn->get_list_of_members()[i]->get_nick() != usr->get_nick())
      			svr.send_message("From " + usr->get_nick() + ":\n" + str + "\n", \
						chn->get_list_of_members()[i]->get_fd());
	  	chn->rmv_member(usr);
	  }
  }
  svr.delete_user(poll_fd_pos);
}
