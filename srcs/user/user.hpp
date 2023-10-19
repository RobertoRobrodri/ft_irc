/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:26:16 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/19 17:02:54 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP
#include <string>
#include <iostream>
#include "../server/server.hpp"
#include "../channel/channel.hpp"

class 	server;
class 	channel;
class	user
{

	private:

		int 			_fd;
		std::string 	_username;
		std::string 	_nick;
		std::string 	_hostname;
		std::string		_realname;
		std::string		_servername;
		bool			_is_registered;
		int 			_n_channels;

	public:

		user 			( void );
		user 			( int fd, std::string hostname);
		user 			( const user & var );
		~user			( void );
		user &operator=	(const user &tmp);

		/*###########################################
		#				GETTERS						#
		############################################*/
		std::string get_username(void) const;
		std::string get_nick(void) const;
		std::string get_hostname(void) const;
		std::string get_servername(void) const;
		std::string get_realname(void) const;
		int 		get_fd(void) const;
		bool 		get_is_registered(void) const;
		int 		get_n_channels(void) const;

		/*###########################################
		#				SETTERS						#
		############################################*/
		void		set_username(std::string username);
		void		set_nick(std::string nick);
		void		set_hostname(std::string host);
		void		set_servername(std::string srv);
		void		set_realname(std::string realname);
		void		set_fd(int fd);
		void		set_is_registered(bool is_registered);
		void 		set_n_channels(int i);
		
		/*###########################################
		#				FUNCTIONS					#
		############################################*/
		void		is_registered(server &svr);
		void		send_to_channel(server &svr, channel *chn, std::string chn_name, std::string msg);
};
std::ostream &operator<<(std::ostream& os, const user &tmp);

#endif
