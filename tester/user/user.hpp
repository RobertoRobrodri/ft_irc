#ifndef USER_HPP
#define USER_HPP
#include <string>
#include <iostream>
#include "../server/server.hpp"
#include "../channel/channel.hpp"

class 	server;
class 	channel;
class	user {

	private:

		int 			_fd;
		std::string 	_username;
		std::string 	_nick;
		std::string 	_hostname;
		std::string		_realname;
		std::string		_servername;
		bool			_is_registered;

	public:

		user 			( void );
		user 			( int fd, std::string hostname );
		user 			( const user & var );
		~user			( void );
		user &operator=	(const user &tmp);

		/*###########################################
		#				GETTERS						#
		############################################*/
		std::string get_username(void) const	{return(this->_username);};
		std::string get_nick(void) const 		{return(this->_nick);};
		std::string get_hostname(void) const 	{return(this->_hostname);};
		std::string get_servername(void) const 	{return(this->_servername);};
		std::string get_realname(void) const 	{return(this->_realname);};
		int 		get_fd(void) const 			{return(this->_fd);};
		bool 		get_is_registered(void) const 			{return(this->_is_registered);};
		
		/*###########################################
		#				SETTERS						#
		############################################*/
		void		set_username(std::string username) {this->_username = username;};
		void		set_nick(std::string nick) {this->_nick = nick;};
		void		set_hostname(std::string host) {this->_hostname = host;};
		void		set_servername(std::string srv) {this->_servername = srv;};
		void		set_realname(std::string realname) {this->_realname = realname;};
		void		set_fd(int fd) {this->_fd = fd;};
		void		set_is_registered(bool is_registered) {this->_is_registered = is_registered;};
		/*###########################################
		#				FUNCTIONS					#
		############################################*/
		void		is_registered(server &svr);
		void		send_to_channel(channel *chn, server &svr, std::string chn_name, std::string msg);
};
std::ostream &operator<<(std::ostream& os, const user &tmp);

#endif
