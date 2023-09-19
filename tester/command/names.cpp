#include "command.hpp"
#include "reply.hpp"

void cmd::names(server &svr, int poll_fd_pos, std::string str)
{
    poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
    user &usr = svr.get_user(pollfd.fd);

    if (usr.get_is_registered() == true)
    {
        if (str == "NAMES")
        {
            std::map<std::string, channel> chnlist = svr.get_list_of_channels();
            std::string secret_lst;
            for (std::map<std::string, channel>::iterator it = chnlist.begin(); it != chnlist.end(); it++)
            {
                channel chn = it->second;
                std::vector<user> members = chn.get_list_of_members();
                //std::cout << "MAZO MODO: " << chn.get_mode() << std::endl;
                if (chn.get_mode().find("sp") != std::string::npos)
                {
                    std::string members_lst;
                    for (std::vector<user>::iterator itt = members.begin(); itt != members.end(); itt++)
                        members_lst += (itt->get_nick() + " ");
                    svr.send_message(RPL_NAMREPLY(it->first, members_lst), usr.get_fd());
                }
                else
                    for (std::vector<user>::iterator itt = members.begin(); itt != members.end(); itt++)
                        if (secret_lst.find(itt->get_nick()) == std::string::npos)
                            secret_lst += (itt->get_nick() + " ");
            }
            std::string schn = "*";
            svr.send_message(RPL_NAMREPLY(schn, secret_lst), usr.get_fd());
            return ;
        }
        std::vector<std::string> chnlist = ft_split(str, ',');
        for (size_t i = 0; i < chnlist.size(); i++)
        {
            channel *chn = svr.get_channel_from_name(chnlist[i]);
            if (!chn->is_user_in_channel(usr))
            {
                if (chn->get_mode().find("sp") != std::string::npos)
                {
                    std::vector<user> members = chn->get_list_of_members();
                    std::string members_lst;
                    for (std::vector<user>::iterator it = members.begin(); it != members.end(); it++)
                        members_lst += (it->get_nick() + " ");
                    svr.send_message(RPL_NAMREPLY(chnlist[i], members_lst), usr.get_fd());
                }
            }
            else
            {
                std::vector<user> members = chn->get_list_of_members();
                std::string members_lst;
                for (std::vector<user>::iterator it = members.begin(); it != members.end(); it++)
                    members_lst += (it->get_nick() + " ");
                svr.send_message(RPL_NAMREPLY(chnlist[i], members_lst), usr.get_fd());
            }
        }
    }
    svr.send_message(RPL_ENDOFNAMES(str), usr.get_fd());
}