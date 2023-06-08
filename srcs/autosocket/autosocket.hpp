#ifndef AUTOSOCKET_HPP
#define AUTOSOCKET_HPP
#include <string>
#include <iostream>
#include "../server/server.hpp"

class	autosocket {

	private:
		sock_in				addr;
		autosocket 			( void );
	public:
		int 				fd;

		autosocket 			( std::string port, std::string host );
		autosocket 			( const autosocket & var );
		~autosocket			( void );

		autosocket &operator=	(const autosocket &tmp);
		void	init_socket_struct(std::string port, std::string host);
		bool	server_listening(void);
};
std::ostream &operator<<(std::ostream& os, const autosocket &tmp);

#endif
