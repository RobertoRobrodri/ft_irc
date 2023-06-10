#include "user.hpp"

user::user( void ) {

  std::cout << "Default constructor called" << std::endl;
  return ;
}

user::user( std::string user, std::string nick, std::string pass ) : _username(user), _nick(nick), _pass(pass) {

  std::cout << "Parameter constructor called" << std::endl;
  return ;
}

user::user( const user & var ) {
  
  std::cout << "Copy constructor called" << std::endl;
  *this = var;
  return ;
  
}

user::~user( void ) {
  
  std::cout << "Destructor called" << std::endl;
  return ;

}

user & user::operator=(const user &tmp) {

  (void) tmp;
  std::cout << "Operator equalizer called" << std::endl;
  return (*this);
  
}

std::ostream &operator<<(std::ostream& os, const user &tmp) {

  os << "Username       |     " << tmp.get_username() << std::endl;
	os << "Nickname       |     " << tmp.get_nick() << std::endl;
	os << "Password       |     " << tmp.get_pass() << std::endl;
	return (os);
}
