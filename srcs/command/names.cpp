/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:16:16 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/24 18:00:39 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "reply.hpp"

void cmd::names(server &svr, int poll_fd_pos, std::string str)
{
    poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
    user *usr = svr.get_user(pollfd.fd);
    std::vector<std::string> dupes;

    if (usr->get_is_registered() == false)
        return svr.send_message(ERR_NOTREGISTERED, usr->get_fd());
    if (str == "")
    {
        std::map<std::string, channel*> chnlist = svr.get_list_of_channels();
        std::string secret_lst;
        for (std::map<std::string, channel*>::iterator it = chnlist.begin(); it != chnlist.end(); it++)
        {
            channel *chn = it->second;
            std::vector<user*> members = chn->get_list_of_members();
            if (chn->is_user_in_channel(usr) || ((chn->get_mode().find("p") == std::string::npos) && (chn->get_mode().find("s") == std::string::npos)))
            {
                std::string members_lst;
                for (std::vector<user*>::iterator itt = members.begin(); itt != members.end(); itt++)
				{
					if (chn->is_user_operator(*itt))
						members_lst += "@";
					members_lst += ((*itt)->get_nick() + " ");
                    dupes.push_back((*itt)->get_nick());
				}
                svr.send_message(RPL_NAMREPLY(it->first, members_lst), usr->get_fd());
            }
            else
            {
                for (std::vector<user*>::iterator itt = members.begin(); itt != members.end(); itt++)
                {
                    if (std::find(dupes.begin(), dupes.end(), (*itt)->get_nick()) == dupes.end())
                    {
                        secret_lst += ((*itt)->get_nick() + " ");
                        dupes.push_back((*itt)->get_nick());
                    }
                }
            }
        }
        std::string schn = "*";
        std::map<int, user*> users_lst = svr.get_list_of_users();
        std::map<int, user*>::iterator user_it;
        for (user_it = users_lst.begin(); user_it != users_lst.end(); user_it++)
        {
            if (user_it->second->get_n_channels() == 0 && user_it->second->get_is_registered() == true)
            {
                if (std::find(dupes.begin(), dupes.end(), user_it->second->get_nick()) == dupes.end())
                {
                        secret_lst += (user_it->second->get_nick() + " ");
                        dupes.push_back(user_it->second->get_nick());
                }
            }
        }
        svr.send_message(RPL_NAMREPLY(schn, secret_lst), usr->get_fd());
        svr.send_message(RPL_ENDOFNAMES(str), usr->get_fd());
        return ;
    }
    std::vector<std::string> chnlist = ft_split(str, ',');
    for (size_t i = 0; i < chnlist.size(); i++)
    {
        channel *chn = svr.get_channel_from_name(chnlist[i]);
        if (chn)
        {
            if (chn->is_user_in_channel(usr) || ((chn->get_mode().find("p") == std::string::npos) && (chn->get_mode().find("s") == std::string::npos)))
            {
                std::vector<user*> members = chn->get_list_of_members();
                std::string members_lst;
                for (std::vector<user*>::iterator it = members.begin(); it != members.end(); it++)
				{
					if (chn->is_user_operator(*it))
						members_lst += "@";
                    members_lst += ((*it)->get_nick() + " ");
				}
				svr.send_message(RPL_NAMREPLY(chnlist[i], members_lst), usr->get_fd());
            }
        }
    }
    svr.send_message(RPL_ENDOFNAMES(str), usr->get_fd());
}
