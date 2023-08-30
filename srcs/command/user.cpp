#include "command.hpp"
#include "reply.hpp"

/* 
 * Parameters: <username> <hostname> <servername> <realname>
 *
 * The USER message is used at the beginning of connection to specify
 * the username, hostname, servername and realname of s new user.
*/

void  cmd::username(server &svr, int poll_fd_pos, std::string str) {
  std::string command =  "USER";
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);

  std::vector<std::string> realname_split = ft_split(str, ':');// Separate realname 
  std::vector<std::string> first_params_split = ft_split(realname_split[0], ' ');
  if (first_params_split.size() + 1 < 4 || realname_split.size() < 2)
  {
    svr.send_message(ERR_NEEDMOREPARAMS(command), usr.get_fd());
    return ;
  }
  
  std::string username = first_params_split[0];
  std::string hostname = first_params_split[1];
  std::string servername = first_params_split[2];
  std::string realname = realname_split[1];

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
  usr.set_username(username);
  usr.set_hostname(hostname);
  usr.set_servername(servername);
  usr.set_realname(realname); 
  usr.is_registered(svr);
}
