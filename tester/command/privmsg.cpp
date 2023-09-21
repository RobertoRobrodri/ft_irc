#include "command.hpp"
#include "reply.hpp"

/*
   Parameters: <receiver>{,<receiver>} <text to be sent>

   PRIVMSG is used to send private messages between users.
   <receiver> is the nickname of the receiver of the message.
   <receiver> can also be a list of names or channels separated with commas.
   
   <receiver> may also me a host mask  (#mask)  or  server mask  ($mask).
   In  both cases the server will only send the PRIVMSG to those who have a
   server or host matching the mask. The mask must have at  least one  "."  
   in it and no wildcards following the last ".".

   Numeric Replies:

           ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
           ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
           ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
           ERR_NOSUCHNICK
           RPL_AWAY

 */

void  cmd::privmsg(server &svr, int poll_fd_pos, std::string str) {
	std::string command = "PRIVMSG";
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  if (usr.get_is_registered() == true)
  {
    if (str == "")
    {
      svr.send_message(ERR_NORECIPIENT(command), usr.get_fd());
      return ;
    }
    std::vector<std::string> msglist = ft_split(str, ' ');
    if (msglist.size() == 1)
    {
      svr.send_message(ERR_NOTEXTTOSEND, usr.get_fd());
      return ;
    }
    std::vector<std::string> rcvlist = ft_split(msglist[0], ',');
    std::string msg = str.substr(str.find(msglist[1]));
	std::sort(rcvlist.begin(), rcvlist.end());
	for (size_t i = 0; i < rcvlist.size(); i++)
    {
		if (i != 0 && rcvlist[i] == rcvlist[i - 1])
		{
          	svr.send_message(ERR_TOOMANYTARGETS(rcvlist[i]), usr.get_fd());
			return;
		}
		if ((rcvlist[i][0] == '#') || (rcvlist[i][0] == '&'))
		{
        	channel *chn = svr.get_channel_from_name(rcvlist[i]);
        	usr.send_to_channel(svr, chn, rcvlist[i], msg);
      	}
		else
		{
        	user *receiver = svr.get_user_from_nick(rcvlist[i]);
        	if (receiver)
          		svr.send_message("From " + usr.get_nick() + ":\n" + msg + "\n", receiver->get_fd());
        	else
          		svr.send_message(ERR_NOSUCHNICK(rcvlist[i]), usr.get_fd());
      	}
    }
  }
  else
	  svr.send_message(ERR_NOTREGISTERED, usr.get_fd());
}

void	test_privmsg_cmd(server *server)
{
	// TO DO: make sure that the message is only reaching the correct receiver
	std::cout << BLUE << "Test privmsg command\n";
	std::cout << "==========================\n" << RESET;

	cmd::username(*server, 1, "user1 hostname1 servername1 :angel");
	cmd::username(*server, 2, "user2 hostname2 servername2 :wiz");
	cmd::nick(*server, 1, "Angel");
	cmd::nick(*server, 2, "Wiz");

	std::cout << CYAN << "Test 1: Simple message\n" << RESET;
	std::cout << YELLOW << "PRIVMSG Wiz :Hello are you receiving this message ?\n" << RESET;
	cmd::privmsg(*server, 1, "Wiz :Hello are you receiving this message ?");

	std::cout << CYAN << "Test 2: Complex message\n" << RESET;
	std::cout << YELLOW << "PRIVMSG Angel :yes I'm receiving it !receiving it !'u>(768u+1n) .br\n" << RESET;
	cmd::privmsg(*server, 2, "Angel :yes I'm receiving it !receiving it !'u>(768u+1n) .br");

	std::cout << CYAN << "Test 3: Message to non existent nick\n" << RESET;
	std::cout << YELLOW << "PRIVMSG somebody :hElp\n" << RESET;
	cmd::privmsg(*server, 1, "somebody :hElp");

	std::cout << CYAN << "Test 4: Empty message text\n" << RESET;
	std::cout << YELLOW << "PRIVMSG Angel\n" << RESET;
	cmd::privmsg(*server, 2, "Angel");
}	
