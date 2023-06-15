#include "command.hpp"

// NICKNAME

// TODO https://modern.ircdocs.horse/#errerroneusnickname-432
void cmd::nick(server &svr, int poll_fd_pos, std::string name) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  //get reference of the user
  user &usr = svr.get_user(pollfd.fd);
  //set nick
  if (svr.compare_nicknames(name) == 0)
  {
	name.append(":Nickname is already in use");
	svr.send_message(const_cast<char *>(name.c_str()), usr.get_fd(), name.length());
	return ;
  }
  usr.set_nick(name);
  std::cout << usr << std::endl;
}

void  cmd::username(server &svr, int poll_fd_pos, std::string name) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  usr.set_username(name);
  std::cout << usr << std::endl;
}

void  cmd::pingpong(server &svr, int poll_fd_pos, std::string name) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  svr.send_message(const_cast<char *>(name.c_str()), usr.get_fd(), name.length());
}

void  cmd::quit(server &svr, int poll_fd_pos, std::string name) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  svr.send_message(const_cast<char *>(name.c_str()), usr.get_fd(), name.length());
  svr.delete_user(poll_fd_pos);
}

void  cmd::privmsg(server &svr, int poll_fd_pos, std::string name) {
  std::vector<std::string> seglist = ft_split(name, ' ');
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  std::map<int, user> lst = svr.get_list_of_users();
  std::cout << "ENTRÉ" << std::endl;
  
  for (std::map<int, user>::iterator it = lst.begin(); it != lst.end(); it++)
	{
		if (it->second.get_nick().compare(seglist[0]) == 0)
    {
      svr.send_message(const_cast<char *>(seglist[1].c_str()), it->second.get_fd(), seglist[1].length());
      svr.send_message(const_cast<char *>("\n"), it->second.get_fd(), 1);
    }
  }
}
