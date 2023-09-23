#include "command.hpp"
#include "reply.hpp"
/*
 	CHANNEL MODES
   Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]

   Allows usernames and channels to have their mode changed by server or operators.
   When parsing MODE messages, it is recommended that the entire message be parsed
   first and then the changes which resulted then passed on.

   Modes available for channels:

           o - give/take channel operator privileges;
           i - invite-only channel flag;
           t - topic settable by channel operator only flag;
		   p - set channel to private;
		   r - set channel to secret;
           l - set the user limit to channel;
           k - set a channel key (password);

   When using the 'o' and 'b' options, a restriction on a total of three
   per mode command has been imposed.

 	USER MODES
   Parameters: <nickname> {[+|-]|i|w|s|o}

   These affect either how the client is seen by others or what 'extra'
   messages the client is sent.
   A user MODE command may only be accepted if both the sender of the
   message and the nickname given as a parameter are both the same.

   The available modes are as follows:
           o - operator flag.
   If a user attempts to make themselves an operator using the "+o"
   flag, the attempt should be ignored.  There is no restriction,
   however, on anyone `deopping' themselves (using "-o").
   
   Numeric Replies:

           ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
           ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_KEYSET
           RPL_BANLIST                     RPL_ENDOFBANLIST
           ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL
           ERR_USERSDONTMATCH              RPL_UMODEIS
           ERR_UMODEUNKNOWNFLAG
*/
void cmd::mode(server &svr, int poll_fd_pos, std::string str)
{
	std::string command = "MODE";
	poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  	user &usr = svr.get_user(pollfd.fd);
	if (usr.get_is_registered() == false)
	{
		svr.send_message(ERR_NOTREGISTERED, usr.get_fd());
		return ;
	}
	std::vector<std::string> msglist = ft_split(str, ' ');
	if (msglist.size() < 2)
	{
  	  	svr.send_message(ERR_NEEDMOREPARAMS(command), usr.get_fd());
  	  	return;
  	}
	channel *chn = svr.get_channel_from_name(msglist[0]);
	if (chn == NULL)
	{
		svr.send_message(ERR_NOSUCHCHANNEL(msglist[0]), usr.get_fd());
		return;
	}
	if (chn->is_user_in_channel(usr) == false)
	{
		svr.send_message(ERR_NOTONCHANNEL(msglist[0]), usr.get_fd());
		return;
	}
	if (chn->is_user_operator(usr) == false)
	{
		svr.send_message(ERR_CHANOPRIVSNEEDED(msglist[0]), usr.get_fd());
			return;
	}
	std::vector<std::string> mode_params = msglist;
	mode_params.erase(mode_params.begin(), mode_params.begin() + 2);
	chn->parse_mode_flag(usr, msglist[1], mode_params, svr);
}

void	test_mode_cmd(server *server)
{
	std::cout << BLUE << "Test mode command\n";
	std::cout << "==========================\n" << RESET;
	
	cmd::username(*server, 1, "user1 hostname1 servername1 :thelma");
	cmd::username(*server, 2, "user2 hostname2 servername2 :louise");
	cmd::nick(*server, 1, "nick_1");
	cmd::nick(*server, 2, "nick_2");
	user usr1 = server->get_user(4);
	user usr2 = server->get_user(5);
	cmd::join(*server, 1, "#TestChannel");
	cmd::join(*server, 2, "#TestChannel");
    channel *channel1 = server->get_channel_from_name("#TestChannel");
	
	std::cout << CYAN << "Test 1: Make channel private and invite-only\n" << RESET;
	std::cout << YELLOW << "MODE #TestChannel +ip\n" << RESET;
	cmd::mode(*server, 1, "#TestChannel +ip");
	std::cout << *channel1 << std::endl;
	
	std::cout << CYAN << "Test 2: Give 'chanop' privileges to nick_2 on channel #TestChannel\n" << RESET;
	std::cout << YELLOW << "MODE #TestChannel +o nick_2\n" << RESET;
	cmd::mode(*server, 1, "#TestChannel +o nick_2");
	std::cout << CYAN << "Test 3: Remove invite-only flag\n" << RESET;
	std::cout << YELLOW << "MODE #TestChannel -i\n" << RESET;
	cmd::mode(*server, 1, "#TestChannel -i");
	std::cout << *channel1 << std::endl;

	std::cout << CYAN << "Test 4: Set channel key to \"secretkey\"\n" << RESET;
	std::cout << YELLOW << "MODE #TestChannel +k secretkey\n" << RESET;
	cmd::mode(*server, 1, "#TestChannel +k secretkey");
	std::cout << *channel1 << std::endl;

	std::cout << CYAN << "Test 5: Set limit of users on channel to 10\n" << RESET;
	std::cout << YELLOW << "MODE #TestChannel +l 10\n" << RESET;
	cmd::mode(*server, 1, "#TestChannel +l 10");
	std::cout << *channel1 << std::endl;

	std::cout << CYAN << "Test 6: nick_1 deopping themselve\n" << RESET;
	std::cout << YELLOW << "MODE -o nick_1\n" << RESET;
	cmd::mode(*server, 1, "-o nick_1");
}