/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:22:31 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/23 12:54:33 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		std::vector<user*> 		list_of_members;
		std::map<user*, bool>	list_of_operators;
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

		std::string get_name(void) const;
		std::string get_topic(void) const;
		std::vector<user*> get_list_of_members(void) const;
		std::string get_password(void) const;
		std::string get_mode(void) const;
		size_t get_user_limit(void) const;
		user   *get_user_from_nick(std::string name);

		/*###########################################
		#				SETTERS						#
		############################################*/

		void set_topic(std::string topic);
		void set_password(std::string pass);
		void set_mode(std::string mode);
		void set_user_limit(size_t i);
		void set_user_operator(user *usr, const bool &flag);

		/*###########################################
		#				FUNCTIONS					#
		############################################*/

		void	add_member(user *usr);
		void	rmv_member(user *usr);
		bool	is_user_in_channel(user *usr);
		bool 	is_user_operator(user *usr);
		int 	parse_mode_flag(user *usr, std::string &modes, std::vector<std::string> mode_params, server &srv);
};
std::ostream &operator<<(std::ostream& os, const channel &tmp);

#endif
