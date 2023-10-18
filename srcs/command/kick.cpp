/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:15:14 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/04 14:36:00 by mzomeno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
  
  if (usr.get_is_registered() == false)
	  return svr.send_message(ERR_NOTREGISTERED, usr.get_fd());

  if (str == "")
      return svr.send_message(ERR_NEEDMOREPARAMS(command), usr.get_fd());
  
  std::vector<std::string> msglist = ft_split(str, ' ');
  if (msglist.size() < 2)
    return svr.send_message(ERR_NEEDMOREPARAMS(command), usr.get_fd());
  std::vector<std::string> chnlist = ft_split(msglist[0], ',');
  std::vector<std::string> usrlist = ft_split(msglist[1], ',');
  for (size_t i = 0; i < chnlist.size(); i++)
  {
	  channel *chn = svr.get_channel_from_name(chnlist[i]);
	  if (!chn)
	  {
          svr.send_message(ERR_NOSUCHNICK(chnlist[i]), usr.get_fd());
		  continue;
	  }
	  if (!chn->is_user_in_channel(usr))
	  {
		  svr.send_message(ERR_NOTONCHANNEL(chnlist[i]), usr.get_fd());
		  continue ;
	  }
	  if (chn->is_user_operator(usr) == false)
	  {
		  svr.send_message(ERR_CHANOPRIVSNEEDED(chnlist[i]), usr.get_fd());
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
