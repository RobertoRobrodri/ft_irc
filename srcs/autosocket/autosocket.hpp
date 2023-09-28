/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autosocket.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:22:56 by crisfern          #+#    #+#             */
/*   Updated: 2023/09/28 11:23:10 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOSOCKET_HPP
#define AUTOSOCKET_HPP
#include <string>
#include <iostream>
#include "../server/server.hpp"

class	autosocket {

	public:
		sock_in				addr;
		autosocket 			( void );
		int 				fd;

		autosocket 			( std::string port, std::string host );
		autosocket 			( const autosocket & var );
		~autosocket			( void );

		autosocket &operator=(const autosocket &tmp);
		void	init_socket_struct(std::string port, std::string host);
		bool	server_listening(void);
};
std::ostream &operator<<(std::ostream& os, const autosocket &tmp);

#endif
