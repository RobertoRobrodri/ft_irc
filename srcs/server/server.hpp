#ifndef SERVER_HPP
#define SERVER_HPP
#include <string>
#include <iostream>

class	server {

	private:

	public:

		server ( void );
		server ( std::string str );
		server ( const server & var );
		~server ( void );
		server &operator=(const server &tmp);
};

std::ostream &operator<<(std::ostream& os, const server &tmp);

#endif
