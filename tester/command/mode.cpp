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
           p - private channel flag;
           s - secret channel flag;
           i - invite-only channel flag;
           t - topic settable by channel operator only flag;
           n - no messages to channel from clients on the outside;
           m - moderated channel;
           l - set the user limit to channel;
           b - set a ban mask to keep users out;
           v - give/take the ability to speak on a moderated channel;
           k - set a channel key (password).

   When using the 'o' and 'b' options, a restriction on a total of three
   per mode command has been imposed.

 	USER MODES
   Parameters: <nickname> {[+|-]|i|w|s|o}

   These affect either how the client is seen by others or what 'extra'
   messages the client is sent.
   A user MODE command may only be accepted if both the sender of the
   message and the nickname given as a parameter are both the same.

   The available modes are as follows:

           i - marks a users as invisible;
           s - marks a user for receipt of server notices;
           w - user receives wallops;
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

   Examples:

           Use of Channel Modes:

MODE #Finnish +im               ; Makes #Finnish channel moderated and
                                'invite-only'.

MODE #Finnish +o Kilroy         ; Gives 'chanop' privileges to Kilroy on
                                channel #Finnish.

MODE #Finnish +v Wiz            ; Allow WiZ to speak on #Finnish.

MODE #Fins -s                   ; Removes 'secret' flag from channel
                                #Fins.

MODE #42 +k oulu                ; Set the channel key to "oulu".

MODE #eu-opers +l 10            ; Set the limit for the number of users
                                on channel to 10.

MODE &oulu +b                   ; list ban masks set for channel.

MODE &oulu +b *!*@*             ; prevent all users from joining.

MODE &oulu +b *!*@*.edu         ; prevent any user from a hostname
                                matching *.edu from joining.

        Use of user Modes:

:MODE WiZ -w                    ; turns reception of WALLOPS messages
                                off for WiZ.

:Angel MODE Angel +i            ; Message from Angel to make themselves
                                invisible.

MODE WiZ -o                     ; WiZ 'deopping' (removing operator
                                status).  The plain reverse of this
                                command ("MODE WiZ +o") must not be
                                allowed from users since would bypass
                                the OPER command.
*/
void cmd::mode(server &svr, int poll_fd_pos, std::string str)
{
	std::string command = "MODE";
	poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  	user &usr = svr.get_user(pollfd.fd);
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
	chn->parse_mode_flag(msglist[1], mode_params, svr);
}

void	test_mode_cmd(server *server)
{
	std::cout << BLUE << "Test mode command\n";
	std::cout << "==========================\n" << RESET;
	
	cmd::join(*server, 1, "#TestChannel");
    channel *channel1 = server->get_channel_from_name("#TestChannel");

	std::cout << CYAN << "Test 1: \n" << RESET;
	std::cout << YELLOW << "MODE\n" << RESET;
	cmd::topic(*server, 1, "");
	std::cout << *channel1 << std::endl;
}
