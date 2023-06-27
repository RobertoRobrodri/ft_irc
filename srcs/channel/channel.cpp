#include "channel.hpp"

channel::channel( void ) : _name(""), _topic("") {

  std::cout << "Default channel constructor called" << std::endl;
  return ;
}

channel::channel( std::string str ) : _name(str), _topic("Default topic") {
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
  return (*this);
}

std::ostream &operator<<(std::ostream& os, const channel &tmp) {
  std::vector<user>::iterator it;
	std::vector<user> lst = tmp.get_list_of_members();

	os << "Channel name: " << tmp.get_name() << std::endl;
	os << "Channel topic: " << tmp.get_topic() << std::endl;
  os << "List of members: " << std::endl;
	for (it = lst.begin(); it != lst.end(); it++)
    	os << it->get_nick() << std::endl;
	return (os);
}

void	channel::add_member(user &usr)
{
	this->list_of_members.push_back(usr);
  std::cout << *this << std::endl;
}