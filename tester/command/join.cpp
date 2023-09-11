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
        if (chn->get_mode().find("k") != std::string::npos)// Requires password
        {
          if (password.empty())
          {
            svr.send_message(": 475 " + chn->get_name() + ":Cannot join channel (+k) \r\n", usr.get_fd());
            return ;
          }
          if (chn->get_password().compare(password))
          {
            svr.send_message(": 475 " + chn->get_name() + ":Cannot join channel (+k) \r\n", usr.get_fd());
            return ;
          }
        }
        if (chn->get_mode().find("l") != std::string::npos)
        {
          if (chn->get_list_of_members().size() >= chn->get_user_limit())
          {
            svr.send_message(": 471 " + chn->get_name() + ":Cannot join channel (+l) \r\n", usr.get_fd());
            return ;
          }
        }
        if (usr.get_n_channels() >= MAX_NUMBER_OF_CHN)
        {
            svr.send_message(": 405 " + chn->get_name() + ":You have joined too many channels\r\n" , usr.get_fd());
            return ;
        }
        chn->add_member(usr);
      }
}

void  cmd::join(server &svr, int poll_fd_pos, std::string str) {
  // Si no existe canal:
  // SVR->crear_canal()
  // else
  // SVR->add_user_to_channel()
  std::vector<std::string> channel_params = ft_split(str, ' ');
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  if (str == "")
  {
	  svr.send_message(": 461 JOIN :Not enough parameters \r\n", usr.get_fd());
    return ;
  }
  // Me pueden pasar una serie de canales separados por ','
  // TODO Channel_params[1] debe contener las contraseñas, si se utilizan
  std::map<std::string, std::string>channels_and_passwords;
  std::vector<std::string> channels = ft_split(channel_params[0], ',');
  std::vector<std::string>::iterator channel_it;
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
            svr.send_message(": 405 " + it->first + ":You have joined too many channels\r\n" , usr.get_fd());
            return ;
      }
      svr.create_channel(usr, it->first, it->second);
    }
  }
}

void	test_join_cmd(server *server)
{
	std::cout << BLUE << "Test join command\n";
	std::cout << "==========================\n" << RESET;

	cmd::nick(*server, 1, "nick_1");
	cmd::nick(*server, 2, "nick_2");

	std::cout <<  CYAN << "Test 1: Join and create new channel\n" << RESET;
	std::cout << YELLOW << "JOIN #foobar\n" << RESET;
	cmd::join(*server, 1, "#foobar");

	std::cout <<  CYAN << "Test 2: Join and create new channel without & or #\n" << RESET;
	std::cout << YELLOW << "JOIN oofbar\n" << RESET;
	cmd::join(*server, 1, "oofbar");

	std::cout <<  CYAN << "Test 3: Join existing channel\n" << RESET;
	std::cout << YELLOW << "JOIN #foobar\n" << RESET;
	cmd::join(*server, 2, "#foobar");

	std::cout <<  CYAN << "Test 4: Join channel using valid key\n" << RESET;
	std::cout << YELLOW << "JOIN &foo fubar\n" << RESET;
	cmd::join(*server, 1, "&foo fubar");

	std::cout <<  CYAN << "Test 5: Join two channels\n" << RESET;
	std::cout << YELLOW << "JOIN #foo,#bar\n" << RESET;
	cmd::join(*server, 2, "&foo,#bar");
	
	std::cout <<  CYAN << "Test 6: Join two channels, one with password\n" << RESET;
	std::cout << YELLOW << "JOIN #foo,&bar fubar\n" << RESET;
	cmd::join(*server, 1, "#bar,&bar fubar");

/* Failed test stops the rest of the execution
	std::cout << "Test 5: Not enough parameters\n" << RESET;
	std::cout << YELLOW << "JOIN\n" << RESET;
	cmd::join(*server, 1, "");
*/

	std::cout <<  CYAN << "Test 7: Join channel with topic\n" << RESET;
	std::cout << YELLOW << "JOIN #oofbar\n" << RESET;
	cmd::topic(*server, 1, "#oofbar test_topic");
	cmd::join(*server, 2, "#oofbar");
}
