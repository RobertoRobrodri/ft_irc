/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:18:53 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/23 12:47:44 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "reply.hpp"

/*
   Parameters: <receiver>{,<receiver>} <text to be sent>

   PRIVMSG is used to send private messages between users.
   <receiver> is the nickname of the receiver of the message.
   <receiver> can also be a list of names or channels separated with commas.
   
   <receiver> may also me a host mask  (#mask)  or  server mask  ($mask).
   In  both cases the server will only send the PRIVMSG to those who have a
   server or host matching the mask. The mask must have at  least one  "."  
   in it and no wildcards following the last ".".

   Numeric Replies:

           ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
           ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
           ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
           ERR_NOSUCHNICK
           RPL_AWAY

 */

void  cmd::privmsg(server &svr, int poll_fd_pos, std::string str)
{
  std::string command = "PRIVMSG";
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user *usr = svr.get_user(pollfd.fd);
  if (usr->get_is_registered() == true)
  {
    if (str == "")
      return svr.send_message(ERR_NORECIPIENT(command), usr->get_fd());
    std::vector<std::string> msglist = ft_split(str, ' ');
    if (msglist.size() == 1)
      return svr.send_message(ERR_NOTEXTTOSEND, usr->get_fd());
    std::vector<std::string> rcvlist = ft_split(msglist[0], ',');
    std::string msg = str.substr(str.find(msglist[1]));
    std::sort(rcvlist.begin(), rcvlist.end());
    for (size_t i = 0; i < rcvlist.size(); i++)
    {
      if (std::count(rcvlist.begin(), rcvlist.end(), rcvlist[i]) > 1)
      {
        if ((i == 0) || ((i > 0) && rcvlist[i] != rcvlist[i - 1]))
          svr.send_message(ERR_TOOMANYTARGETS(rcvlist[i]), usr->get_fd());
          continue;
      }
      if ((rcvlist[i][0] == '#') || (rcvlist[i][0] == '&'))
      {
        channel *chn = svr.get_channel_from_name(rcvlist[i]);
        if (chn)
          usr->send_to_channel(svr, chn, rcvlist[i], msg);
      }
      else
      {
        user *receiver = svr.get_user_from_nick(rcvlist[i]);
        if (receiver)
        {
          if (receiver->get_nick() != usr->get_nick())
            svr.send_message("From " + usr->get_nick() + ":\n" + msg + "\n", receiver->get_fd());
        }
        else
          svr.send_message(ERR_NOSUCHNICK(rcvlist[i]), usr->get_fd());
      }
    }
  }
  else
    svr.send_message(ERR_NOTREGISTERED, usr->get_fd());
}
