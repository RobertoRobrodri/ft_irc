/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:14:38 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/23 12:41:56 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "reply.hpp"

// Parameters: <channel>{,<channel>} [<key>{,<key>}]

//   is used by client to start listening to an specific channel.
//
//   The server checks if client is allowed to join a channel.
//   The conditions which affect this are:
//		1.  the user must be invited if the channel is invite-only;
//      2.  the user's nick/username/hostname must not match any
//      	active bans;
//      3.  the correct key (password) must be given if it is set.

//   Once a user has joined a channel, they receive notice about all
//   the MODE, KICK, PART and QUIT commands their channel receives.

//   If a JOIN is successful, the user is then sent the channel's topic
//   (using RPL_TOPIC) and the list of users who are on the channel (using
//   RPL_NAMREPLY), which must include the user joining.
//
//   Numeric Replies:
//
//           ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
//           ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
//           ERR_CHANNELISFULL               ERR_BADCHANMASK
//           ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
//           RPL_TOPIC

void	join_existing_channel(server &svr, channel *chn, user *usr, std::string password)
{
	std::string channel = chn->get_name();
	std::string user = usr->get_nick();
	if (chn->is_user_in_channel(usr) == true)
		return svr.send_message(ERR_USERONCHANNEL(user, channel), usr->get_fd());
	if (chn->get_mode().find("i") != std::string::npos)// Invite only channel
		return svr.send_message(ERR_INVITEONLYCHAN(channel), usr->get_fd());
	if (chn->get_mode().find("k") != std::string::npos &&
			chn->get_password().compare(password))// Require password
		return svr.send_message(ERR_BADCHANNELKEY(channel), usr->get_fd());
	if (chn->get_mode().find("l") != std::string::npos &&
			chn->get_list_of_members().size() >= chn->get_user_limit())// Limit of users
		return svr.send_message(ERR_CHANNELISFULL(channel), usr->get_fd());
	if (usr->get_n_channels() >= MAX_NUMBER_OF_CHN)// Limit of channels
		return svr.send_message(ERR_TOOMANYCHANNELS(channel), usr->get_fd());
	chn->add_member(usr);
}

void  cmd::join(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user *usr = svr.get_user(pollfd.fd);
	if (usr->get_is_registered() == true)
  {
    std::string command = "JOIN";
    std::vector<std::string> channel_params = ft_split(str, ' ');
    if (str == "")
      return svr.send_message(ERR_NEEDMOREPARAMS(command), usr->get_fd());
    std::map<std::string, std::string>channels_and_passwords;
    std::vector<std::string> channels = ft_split(channel_params[0], ',');
    std::vector<std::string>::iterator channel_it = channels.begin();
    if (channel_params.size() >= 2) // we have passwords
    {
      std::vector<std::string> passwords = ft_split(channel_params[1], ',');
      std::vector<std::string>::iterator password_it = passwords.begin();
      while (password_it != passwords.end())
      {
        channels_and_passwords.insert(std::pair<std::string, std::string>(*channel_it, *password_it));
        password_it++;
        channel_it++;
      }
    }
    while (channel_it != channels.end())
    {
        channels_and_passwords.insert(std::pair<std::string, std::string>(*channel_it, ""));
        channel_it++;
    }
    for (std::map<std::string, std::string>::iterator it = channels_and_passwords.begin(); it != channels_and_passwords.end(); it++)
    {
		if (it->first[0] != '#' && it->first[0] != '&')
      		return ;
      	channel *chn = svr.get_channel_from_name(it->first);
      if (chn)
        join_existing_channel(svr, chn, usr, it->second);
      else
      {
        if (usr->get_n_channels() >= MAX_NUMBER_OF_CHN)
              return svr.send_message(ERR_TOOMANYCHANNELS(it->first), usr->get_fd());
        svr.create_channel(usr, it->first, it->second);
      }
    }
  }
	else
	  svr.send_message(ERR_NOTREGISTERED, usr->get_fd());
}
