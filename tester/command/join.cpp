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

void	join_existing_channel(server &svr, channel *chn, user &usr, std::string password)
{
	std::string channel = chn->get_name();
	std::string user = usr.get_nick();
	if (chn->is_user_in_channel(usr) == true)
	{
		svr.send_message(ERR_USERONCHANNEL(user, channel), usr.get_fd());
		return ;
	}
	else
	{ 
		if (chn->get_mode().find("i") != std::string::npos)// Invite only channel
        {
          svr.send_message(ERR_INVITEONLYCHAN(channel), usr.get_fd());
			return ;
        } 
        if (chn->get_mode().find("k") != std::string::npos &&// Require password
		   	chn->get_password().compare(password))
        {
            svr.send_message(ERR_BADCHANNELKEY(channel), usr.get_fd());
            return ;
        }
        if (chn->get_mode().find("l") != std::string::npos &&// Limit of users
				chn->get_list_of_members().size() >= chn->get_user_limit())
		{
            svr.send_message(ERR_CHANNELISFULL(channel), usr.get_fd());
            return ;
        }
        if (usr.get_n_channels() >= MAX_NUMBER_OF_CHN)// Limit of channels
        {
            svr.send_message(ERR_TOOMANYCHANNELS(channel), usr.get_fd());
            return ;
        }
        chn->add_member(usr);
	}
}

void  cmd::join(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
	if (usr.get_is_registered() == true)
  {
    std::string command = "JOIN";
    std::vector<std::string> channel_params = ft_split(str, ' ');
    if (str == "")
    {
      svr.send_message(ERR_NEEDMOREPARAMS(command), usr.get_fd());
      return ;
    }
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
      channel *chn = svr.get_channel_from_name(it->first);
      if (chn)
      {
        join_existing_channel(svr, chn, usr, it->second);
      }
      else
      {
        if (usr.get_n_channels() >= MAX_NUMBER_OF_CHN)
        {
              svr.send_message(ERR_TOOMANYCHANNELS(chn->get_name()), usr.get_fd());
              return ;
        }
        svr.create_channel(usr, it->first, it->second);
      }
	  svr.send_message(RPL_TOPIC(chn->get_name(), chn->get_topic()), usr.get_fd());
    }
  }
	else
	  svr.send_message(ERR_NOTREGISTERED, usr.get_fd());
}

void	test_join_cmd(server *server)
{
	std::cout << BLUE << "Test join command\n";
	std::cout << "==========================\n" << RESET;

	cmd::username(*server, 1, "user1 hostname1 servername1 :thelma");
	cmd::username(*server, 2, "user2 hostname2 servername2 :louise");
	cmd::nick(*server, 1, "nick_1");
	cmd::nick(*server, 2, "nick_2");

	std::cout <<  CYAN << "Test 1: Join and create new channel\n" << RESET;
	std::cout << YELLOW << "JOIN #chan1\n" << RESET;
	cmd::join(*server, 1, "#chan1");

	std::cout <<  CYAN << "Test 2: Join and create new channel without & or #\n" << RESET;
	std::cout << YELLOW << "JOIN chan2\n" << RESET;
	cmd::join(*server, 1, "chan2");

	std::cout <<  CYAN << "Test 3: Join existing channel\n" << RESET;
	std::cout << YELLOW << "JOIN #chan1\n" << RESET;
	cmd::join(*server, 2, "#chan1");

	std::cout <<  CYAN << "Test 4: Create channel with pass\n" << RESET;
	std::cout << YELLOW << "JOIN &passchan pass\n" << RESET;
	cmd::join(*server, 1, "&passchan pass");

	std::cout <<  CYAN << "Test 5: Join two channels, one with password\n" << RESET;
	std::cout << YELLOW << "JOIN #foo,&bar fubar\n" << RESET;
	cmd::join(*server, 2, "chan2,&passchan pass");

	std::cout <<  CYAN << "Test 7: Join channel with topic\n" << RESET;
	std::cout << YELLOW << "TOPIC #topic_chan test_topic\nJOIN #topic_chan\n" << RESET;
	cmd::join(*server, 1, "#topic_chan");
	cmd::topic(*server, 1, "#topic_chan test_topic");
	cmd::join(*server, 2, "#topic_chan");

	std::cout  <<  CYAN << "Test 8: Not enough parameters\n" << RESET;
	std::cout << YELLOW << "JOIN\n" << RESET;
	cmd::join(*server, 1, "");
	
	std::cout <<  CYAN << "Test 9: Try to join invite-only channel\n" << RESET;
	std::cout << YELLOW << "JOIN #oofbar\n" << RESET;
	cmd::join(*server, 1, "#restrictive_chan");
	cmd::mode(*server, 1, "#restrictive_chan +i");
	cmd::join(*server, 2, "#restrictive_chan");
}
