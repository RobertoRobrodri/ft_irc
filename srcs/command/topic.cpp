/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:19:39 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/04 12:35:35 by mzomeno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "reply.hpp"

/*
  Parameters: <channel> [<topic>]

   The TOPIC message is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic> given.
   If the <topic> parameter is present, the topic for that channel will be
   changed if the channel modes permit this action.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
           RPL_NOTOPIC                     RPL_TOPIC
           ERR_CHANOPRIVSNEEDED

 */

void	check_topic(server &svr, std::string channel_name, std::string topic, user &usr)
{
	if (topic.empty())
		svr.send_message(RPL_NOTOPIC(channel_name), usr.get_fd());
	else
		svr.send_message(RPL_TOPIC(channel_name, topic), usr.get_fd());
}

void	modify_topic(server &svr, std::string channel_name, std::string topic, channel *chn, user &usr)
{
	if (chn->get_mode().find("t") != std::string::npos && !usr.get_op())
		return svr.send_message(ERR_CHANOPRIVSNEEDED(channel_name), usr.get_fd());
	
	chn->set_topic(topic);
	std::vector<user*>::iterator it;
	std::vector<user*> lst = chn->get_list_of_members();
	for (it = lst.begin(); it != lst.end(); it++)
		check_topic(svr, channel_name, topic, *(*it));
}

void cmd::topic(server &svr, int poll_fd_pos, std::string str)
{
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);

  /* ERROR MESSAGES */
  if (usr.get_is_registered() == false)
	  return svr.send_message(ERR_NOTREGISTERED, usr.get_fd());
  
  std::string command = "TOPIC";
  if (str == "")
      return svr.send_message(ERR_NEEDMOREPARAMS(command), usr.get_fd());
  
  std::vector<std::string> msglist = ft_split(str, ' ');
  channel *chn = svr.get_channel_from_name(msglist[0]);
  if (!chn)
      return svr.send_message(ERR_NOSUCHCHANNEL(msglist[0]), usr.get_fd());
  
  if (chn->is_user_in_channel(usr) == false)
	  return svr.send_message(ERR_NOTONCHANNEL(msglist[0]), usr.get_fd());
  
  /* COMMAND ACCEPTED */
  std::string topic;
  if (msglist.size() == 1)
	  check_topic(svr, msglist[0], chn->get_topic(), usr);
  else
	  modify_topic(svr, msglist[0], str.substr(str.find(msglist[1])), chn, usr);
}
