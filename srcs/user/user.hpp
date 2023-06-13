#ifndef USER_HPP
#define USER_HPP
#include <string>
#include <iostream>
#include "../server/server.hpp"

class	user {

	private:

		int 			_fd;
		std::string 	_username;
		std::string 	_nick;
		std::string 	_pass;

	public:

		user 			( void );
		user 			( int fd );
		user 			( const user & var );
		~user			( void );
		user &operator=	(const user &tmp);

		/*###########################################
		#				GETTERS						#
		############################################*/
		std::string get_username(void) const 		{return(this->_username);};
		std::string get_nick(void) const 			{return(this->_nick);};
		std::string get_pass(void) const 			{return(this->_pass);};
		int 		get_fd(void) const 			{return(this->_fd);};
		/*###########################################
		#				SETTERS						#
		############################################*/
		void		set_username(std::string username) {this->_username = username;};
		void		set_nick(std::string nick) {this->_nick = nick;};
		void		set_pass(std::string pass) {this->_pass = pass;};
		void		set_fd(int fd) {this->_fd = fd;};
};
std::ostream &operator<<(std::ostream& os, const user &tmp);

#endif
