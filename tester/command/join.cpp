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



void  cmd::join(server &svr, int poll_fd_pos, std::string str) {
  // Si no existe canal:
  // SVR->crear_canal()
  // else
  // SVR->add_user_to_channel()
  std::vector<std::string> channel_params = ft_split(str, ' ');
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  if (str == "JOIN")
  {
	  svr.send_message(": 461 JOIN :Not enough parameters \r\n", usr.get_fd());
    return ;
  }
  // Me pueden pasar una serie de canales separados por ','
  // TODO Channel_params[1] debe contener las contraseñas, si se utilizan
  std::vector<std::string> channels_to_join = ft_split(channel_params[0], ',');
  std::vector<std::string> passwords;
  std::vector<std::string>::iterator password_it;
  if (channel_params.size() >= 2)
  {
    passwords = ft_split(channel_params[1], ',');
    password_it = passwords.begin();
  }
  for (std::vector<std::string>::iterator it = channels_to_join.begin(); it != channels_to_join.end(); it++)
  {
    channel *chn = svr.get_channel_from_name(*it);
    if (chn)
    {
      if (chn->is_user_in_channel(usr) == true)
      {
          svr.send_message(": 443 " + usr.get_nick() + " " + chn->get_name() + ": is already on channel \r\n", usr.get_fd());
          continue ;
      }
      else
      {
        // Invite only channel
        if (chn->get_mode().find("i") != std::string::npos)
        {
          svr.send_message(": 473 " + chn->get_name() + ":Cannot join channel (+i \r\n)", usr.get_fd());
          continue ;
        }
        // Requires password
        if (chn->get_mode().find("k") != std::string::npos)
        {
          if (passwords.empty())
          {
            svr.send_message(": 475 " + chn->get_name() + ":Cannot join channel (+k) \r\n", usr.get_fd());
            continue ;
          }
          if (chn->get_password().compare(*password_it++))
          {
            svr.send_message(": 475 " + chn->get_name() + ":Cannot join channel (+k) \r\n", usr.get_fd());
            continue ;
          }
        }
        if (chn->get_mode().find("l") != std::string::npos)
        {
          if (chn->get_list_of_members().size() >= chn->get_user_limit())
          {
            svr.send_message(": 471 " + chn->get_name() + ":Cannot join channel (+l) \r\n", usr.get_fd());
            continue ;
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
    else
    {
      if (usr.get_n_channels() >= MAX_NUMBER_OF_CHN)
      {
            svr.send_message(": 405 " + *it + ":You have joined too many channels\r\n" , usr.get_fd());
            return ;
      }
      svr.create_channel(usr, *it);
    }
  }
}

void	test_join_cmd(server *server)
{
	std::cout << BLUE << "Test join command\n";
	std::cout << "==========================\n" << RESET;

	std::cout << "Test 1: Join channel\n" << RESET;
	std::cout << YELLOW << "JOIN #foobar\n" << RESET;
	cmd::join(*server, 1, "JOIN #foobar");

	std::cout << "Test 2: Join channel using valid key\n" << RESET;
	std::cout << YELLOW << "JOIN &foo fubar\n" << RESET;
	cmd::join(*server, 1, "JOIN &foo fubar");

	std::cout << "Test 3: Join two channels\n" << RESET;
	std::cout << YELLOW << "JOIN #foo,#bar\n" << RESET;
	cmd::join(*server, 1, "JOIN #foo,#bar");
	
	std::cout << "Test 4: Join two channels, one with password\n" << RESET;
	std::cout << YELLOW << "JOIN #foo,&bar fubar\n" << RESET;
	cmd::join(*server, 1, "JOIN #foo,&bar fubar");

	std::cout << "Test 5: Not enough parameters\n" << RESET;
	std::cout << YELLOW << "JOIN\n" << RESET;
	cmd::join(*server, 1, "JOIN");

	std::cout << "Test 6: No & or # in channel name\n" << RESET;
	std::cout << YELLOW << "JOIN badchannel\n" << RESET;
	cmd::join(*server, 1, "JOIN badchannel");

	std::cout << "Test 7: Comma in channel name\n" << RESET;
	std::cout << YELLOW << "JOIN #The,channel\n" << RESET;
	cmd::join(*server, 1, "JOIN #The,channel");
}
