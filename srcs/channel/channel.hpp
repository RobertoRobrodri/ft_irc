#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <string>
#include <iostream>
#include "../server/server.hpp"

class	user;
class	channel {

	private:

		std::string				_name;
		std::string				_topic;
		std::vector<user> 		list_of_members;

		channel 			( void );
	public:

		channel 			( std::string str );
		channel 			( const channel & var );
		~channel			( void );
		channel &operator=	(const channel &tmp);
		/*###########################################
		#				GETTERS						#
		############################################*/
		std::string get_name(void) const 			{return(this->_name);};
		std::string get_topic(void) const 			{return(this->_topic);};
		std::vector<user> get_list_of_members(void) const {return(this->list_of_members);};
		/*###########################################
		#				FUNCTIONS					#
		############################################*/
		void	add_member(user &usr);
};
std::ostream &operator<<(std::ostream& os, const channel &tmp);

#endif