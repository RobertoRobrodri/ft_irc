#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#define MAX_NUMBER_OF_CHN 100	
#include <string>
#include <iostream>
#include "../server/server.hpp"
#include "../command/reply.hpp"
#include <algorithm>

class	user;
class 	server;
class	channel {

	private:

		std::string				_name;
		std::string				_topic;
		std::vector<user> 		list_of_members;
		std::string 			_password;
		std::string 			_mode;
		size_t 					_user_limit;

		channel 			( void );
	public:

		channel 			( std::string str );
		channel 			( std::string c_name, std::string password );
		channel 			( const channel & var );
		~channel			( void );
		channel &operator=	(const channel &tmp);
		/*###########################################
		#				GETTERS						#
		############################################*/
		std::string get_name(void) const 			{return(this->_name);};
		std::string get_topic(void) const 			{return(this->_topic);};
		std::vector<user> get_list_of_members(void) const {return(this->list_of_members);};
		std::string get_password(void) const {return (this->_password);};
		std::string get_mode(void) const {return (this->_mode);};
		size_t get_user_limit(void) const {return (this->_user_limit);};
		user   *get_user_from_nick(std::string name);

		/*###########################################
		#				SETTERS						#
		############################################*/
		void set_topic(std::string topic) {this->_topic = topic;};
		void set_password(std::string pass) {this->_password = pass;};
		void set_mode(std::string mode) {this->_mode = mode;};
		void set_user_limit(size_t i) {this->_user_limit = i;};

		/*###########################################
		#				FUNCTIONS					#
		############################################*/
		void	add_member(user &usr);
		void	rmv_member(user &usr);
		bool	is_user_in_channel(const user &usr);
		bool 	is_user_operator(const user &usr);
		void	set_user_operator(const user &usr, const bool &flag);
		void	broadcast_channel_mode(std::string &modes, std::vector<std::string> mode_params, server &srv);
		void	broadcast_user_mode(bool sign, server &srv);
		void 	parse_mode_flag(user &usr, std::string &modes, std::vector<std::string> mode_params, server &srv);
};
std::ostream &operator<<(std::ostream& os, const channel &tmp);

#endif
