#include "user.hpp"

user::user( void ) : _fd(0), _username(""), _nick(""), _pass("") {

  std::cout << "User Default constructor called" << std::endl;
  return ;
}

user::user( int fd ) : _fd(fd) {

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
  this->set_pass(tmp.get_pass());
  this->set_fd(tmp.get_fd());
  return (*this);
  
}

std::ostream &operator<<(std::ostream& os, const user &tmp) {
  os << "Username       |     " << tmp.get_username() << std::endl;
	os << "Nickname       |     " << tmp.get_nick() << std::endl;
	os << "Password       |     " << tmp.get_pass() << std::endl;
  os << "FD             |     " << tmp.get_fd() << std::endl;
	return (os);
}