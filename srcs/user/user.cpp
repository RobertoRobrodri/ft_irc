#include "user.hpp"

user::user( void ) : _fd(0), _username(""), _nick(""), _hostname(""), _realname(""), _servername(""), _is_registered(false) {

  std::cout << "User Default constructor called" << std::endl;
  return ;
}

user::user( int fd, std::string hostname ) : _fd(fd), _hostname(hostname) {

  std::cout << "User Parameter constructor called" << std::endl;
  return ;
}

user::user( const user & var ) {
  
  std::cout << "User Copy constructor called" << std::endl;
  *this = var;
  return ;
}

user::~user( void ) {
  
  std::cout << "User Destructor called" << std::endl;
  return ;

}

user & user::operator=(const user &tmp) {

  std::cout << "User Operator equalizer called" << std::endl;
  this->set_username(tmp.get_username());
  this->set_nick(tmp.get_nick());
  this->set_hostname(tmp.get_hostname());
  this->set_realname(tmp.get_realname());
  this->set_servername(tmp.get_servername());
  this->set_fd(tmp.get_fd());
  this->set_is_registered(tmp.get_is_registered());
  return (*this);
  
}

std::ostream &operator<<(std::ostream& os, const user &tmp) {
  os << "Username       |     " << tmp.get_username() << std::endl;
	os << "Nickname       |     " << tmp.get_nick() << std::endl;
	os << "Hostname       |     " << tmp.get_hostname() << std::endl;
  os << "Servername     |     " << tmp.get_servername() << std::endl;
	os << "Realname       |     " << tmp.get_realname() << std::endl;
  os << "FD             |     " << tmp.get_fd() << std::endl;
  os << "User registered|     " << tmp.get_is_registered() << std::endl;
	return (os);
}

void  user::is_registered(server &svr)
{
  if (!this->get_username().empty() && !this->get_nick().empty() && !this->_is_registered)
  {
    this->_is_registered = true;
    svr.send_message(": 001 " + this->get_nick() + " : welcome " + this->get_nick() + "\r\n", this->get_fd());
  }
}