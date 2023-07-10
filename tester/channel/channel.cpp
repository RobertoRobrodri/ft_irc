#include "channel.hpp"

channel::channel( void ) : _user_limit(0) {

  std::cout << "Default channel constructor called" << std::endl;
  return ;
}

channel::channel( std::string str ) : _name(str), _user_limit(0) {
  std::cout << "Parameter channel constructor called" << std::endl;
  return ;
}

channel::channel( const channel & var ) {
  
  std::cout << "Copy channel constructor called" << std::endl;
  *this = var;
  return ;
  
}

channel::~channel( void ) {
  
  std::cout << "Destructor channel called" << std::endl;
  return ;

}

channel & channel::operator=(const channel &tmp) {
  std::cout << "Operator equalizer channel called" << std::endl;
  this->_name = tmp.get_name();
  this->_topic = tmp.get_topic();
  this->list_of_members = tmp.list_of_members;
  this->_password = tmp.get_password();
  this->_mode = tmp.get_mode();
  this->_user_limit = tmp.get_user_limit();
  return (*this);
}

std::ostream &operator<<(std::ostream& os, const channel &tmp) {
  std::vector<user>::iterator it;
	std::vector<user> lst = tmp.get_list_of_members();

	os << "Channel name: " << tmp.get_name() << std::endl;
	os << "Channel topic: " << tmp.get_topic() << std::endl;
  os << "Channel password: "<< tmp.get_password() << std::endl;
  os << "Channel mode: " << tmp.get_mode() << std::endl;
  os << "User limit if set: " << tmp.get_user_limit() << std::endl;
  os << "List of members: " << std::endl;
	for (it = lst.begin(); it != lst.end(); it++)
    	os << it->get_nick() << std::endl;
	return (os);
}

void	channel::add_member(user &usr)
{
  usr.set_n_channels(usr.get_n_channels() + 1);
	this->list_of_members.push_back(usr);
  std::string members;
  std::cout << "Join successful! " << std::endl;
  std::cout << usr << std::endl;
  //RPL_TOPIC
  //server::send_message(":332 " + this->get_name() + " :" + this->get_topic() + "\r\n", usr.get_fd());
  //RPL_NAMREPLY
  for (std::vector<user>::iterator it = this->list_of_members.begin(); it != this->list_of_members.end(); it++)
    members += (it->get_nick() + " ");
  
  //server::send_message(":353 " + members + "\r\n", usr.get_fd());
}

void	channel::rmv_member(user &usr)
{
  std::vector<user>::iterator it;
  for (it = this->list_of_members.begin(); it != this->list_of_members.end(); it++)
  {
    if (it->get_nick() == usr.get_nick())
    {
	    this->list_of_members.erase(it);
      usr.set_n_channels(usr.get_n_channels() - 1);
      return ;
    }
  }
}

bool	channel::is_user_in_channel(const user &usr)
{
  for (std::vector<user>::iterator it = this->list_of_members.begin(); it != this->list_of_members.end(); it++)
  {
    if (it->get_nick() == usr.get_nick())
      return true;
  }
  return false;
}
