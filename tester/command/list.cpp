#include "command.hpp"
#include "reply.hpp"

void cmd::list(server &svr, int poll_fd_pos, std::string str)
{
    poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
    user &usr = svr.get_user(pollfd.fd);
    std::string privchn = "pvr";
    std::string nprivchn = "";

    svr.send_message(RPL_LISTSTART, usr.get_fd());
    if (usr.get_is_registered() == true)
    {
        if (str == "LIST")
        {
            std::map<std::string, channel> chnlist = svr.get_list_of_channels();
            for (std::map<std::string, channel>::iterator it = chnlist.begin(); it != chnlist.end(); it++)
            {
                channel chn = it->second;
                if ((chn.get_mode().find('p') != std::string::npos) && (chn.get_mode().find('s') != std::string::npos) && chn.is_user_in_channel(usr))
                    svr.send_message(RPL_LIST(it->first, privchn, chn.get_topic()), usr.get_fd());
                else if (chn.get_mode().find('p') != std::string::npos)
                {
                    if (chn.is_user_in_channel(usr))
                        svr.send_message(RPL_LIST(it->first, privchn, chn.get_topic()), usr.get_fd());
                    else
                        svr.send_message(RPL_LIST(it->first, privchn, nprivchn), usr.get_fd());
                }
                else if ((chn.get_mode().find('s') != std::string::npos) && chn.is_user_in_channel(usr) || (chn.get_mode().find('s') == std::string::npos))
                {
                    svr.send_message(RPL_LIST(it->first, nprivchn, chn.get_topic()), usr.get_fd());
                }
            }
        }
        else
        {
            std::vector<std::string> chnlist = ft_split(str, ',');
            for (size_t i = 0; i < chnlist.size(); i++)
            {
                channel *chn = svr.get_channel_from_name(chnlist[i]);
                if (chn)
                {
                    if ((chn->get_mode().find('s') != std::string::npos) && (chn->get_mode().find('p') != std::string::npos) && chn->is_user_in_channel(usr))
                        svr.send_message(RPL_LIST(chnlist[i], privchn, chn->get_topic()), usr.get_fd());
                    else if (chn->get_mode().find('p') != std::string::npos)
                    {
                        if (chn->is_user_in_channel(usr))
                            svr.send_message(RPL_LIST(chnlist[i], privchn, chn->get_topic()), usr.get_fd());
                        else
                            svr.send_message(RPL_LIST(chnlist[i], privchn, nprivchn), usr.get_fd());
                    }
                    else if ((chn->get_mode().find('s') != std::string::npos) && chn->is_user_in_channel(usr) || (chn->get_mode().find('s') == std::string::npos))
                        svr.send_message(RPL_LIST(chnlist[i], nprivchn, chn->get_topic()), usr.get_fd());
                }
            }
        }
        svr.send_message(RPL_LISTEND, usr.get_fd());
    }
}
