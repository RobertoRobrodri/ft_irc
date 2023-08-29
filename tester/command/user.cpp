#include "command.hpp"
#include "reply.hpp"

//  Parameters: <username> <hostname> <servername> <realname>
//  
//  The USER message is used at the beginning of connection to specify
//    the username, hostname, servername and realname of a new user.
//    Ref: https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.3
//
//	  realname may contain spaces
//
//	  ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
//
//  hostname and servername are ignored by the server when the command 
//  comes from a directly connected client

void  cmd::username(server &svr, int poll_fd_pos, std::string str) {
  std::string command =  "USER";
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);

  std::vector<std::string> cmd_params = ft_split(str, ':');// Separar el resto del realname 
  std::vector<std::string> other_params = ft_split(cmd_params[0], ' ');
  if (other_params.size() + 1 < 4 || cmd_params.size() < 2)
  {
    svr.send_message(ERR_NEEDMOREPARAMS(command), usr.get_fd());
    return ;
  }
  
  std::string realname = cmd_params[1];
  std::string username = other_params[0];
  std::string hostname = other_params[1];
  std::string servername = other_params[2];

  std::map<int, user> users_lst = svr.get_list_of_users();
  std::map<int, user>::iterator it;
  for (it = users_lst.begin(); it != users_lst.end(); it++)
  {
	  if((it->second).get_username() == username)
	  {
		  svr.send_message(ERR_ALREADYREGISTERED, usr.get_fd());
		  return ;
	  }
  }
  usr.set_username(other_params[0]);
  usr.set_hostname(other_params[1]);
  usr.set_servername(other_params[2]);
  usr.set_realname(realname);
 
  // IF registered --> Send RPL_WELCOME
  //TODO meterlo en el define
  usr.is_registered(svr);
  std::cout << usr << std::endl;
}

void	test_user_cmd(server *server)
{
	std::cout << BLUE << "Test user command\n";
	std::cout << "==========================\n" << RESET;
	
	std::cout << CYAN << "Test 1: Valid command user 1\n" << RESET;
	std::cout << YELLOW << "USER guest tolmoon tolsun :Ronnie Reagan\n" << RESET;
	cmd::username(*server, 1, "guest tolmoon tolsun :Ronnie Reagan");
	
	std::cout <<  CYAN << "Test 2: Valid command user 2\n" << RESET;
	std::cout << YELLOW << "USER ronnie tolmoon tolsun :Ronnie Reagan\n" << RESET;
	cmd::username(*server, 2, "ronnie tolmoon tolsun :Ronnie Reagan");
	
	std::cout <<  CYAN << "Test 3: Missing realname parameter\n" << RESET;
	std::cout <<  YELLOW << "USER guest tolmoon tolsun\n" << RESET;
	cmd::username(*server, 1, "guest tolmoon tolsun");
	
	std::cout <<  CYAN << "Test 4: Missing servername parameter\n" << RESET;
	std::cout <<  YELLOW << "USER guest tolmoon :Ronnie Reagan\n" << RESET;
	cmd::username(*server, 1, "guest tolmoon :Ronnie Reagan");
	
	std::cout <<  CYAN << "Test 5: Semicolons in hostname parameter\n" << RESET;
	std::cout <<  YELLOW << "USER guest :tolmoon tolsun :Ronnie Reagan\n" << RESET;
	cmd::username(*server, 1, "guest :tolmoon tolsun :Ronnie Reagan");
	
	std::cout <<  CYAN << "Test 6: Missing semicolon for realname\n" << RESET;
	std::cout <<  YELLOW << "USER guest tolmoon tolsun Ronnie Reagan\n" << RESET;
	cmd::username(*server, 1, "guest tolmoon tolsun Ronnie Reagan");
	
	std::cout <<  CYAN << "Test 7: Special characters in username parameter\n" << RESET;
	std::cout <<  YELLOW << "USER pwg03[}*]g tolmoon tolsun :Ronnie Reagan\n" << RESET;
	cmd::username(*server, 1, "pwg03[}*]g tolmoon tolsun :Ronnie Reagan");
	
	std::cout <<  CYAN << "Test 8: Already registred error\n" << RESET;
	std::cout << YELLOW << "USER ronnie tolmoon tolsun :Ronnie Reagan\n" << RESET;
	cmd::username(*server, 2, "ronnie tolmoon tolsun :Ronnie Reagan");
}
