#include "command.hpp"

// INFERFACE
command::command( void ) {

  std::cout << "Default constructor called" << std::endl;
  return ;
}

command::command( std::string cmd ) : _cmd(cmd) {

  std::cout << "Parameter constructor called" << std::endl;
  return ;
  
}

command::~command( void ) {
  
  std::cout << "Destructor called" << std::endl;
  return ;

}

void	command::execute(int fd, user user, std::string cmd) {
  std::cout << fd << user.get_username() << " did " << cmd << std::endl;
}

// NICKNAME

nick::nick( void ) : command("NICK") {
  std::cout << "Nick default constructor called" << std::endl;
  return ;
}

nick::~nick(void) {
  std::cout << "Nick Destructor called" << std::endl;
  return ;
}

void nick::execute(int fd, user user, std::string cmd) {
	(void)fd;
	user.set_username(cmd);
}  