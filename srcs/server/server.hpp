#ifndef SERVER_HPP
#define SERVER_HPP
#include <string>
#include <iostream>
#include <cstring>

class	server {

	private:
		
		std::string host;
		std::string network_pass;
		std::string network_port;
		std::string port;
		std::string password;

	public:

		server ( void );
		server( std::string network , std::string prt , std::string pass );
		server ( const server & var );
		~server ( void );
		server &operator=(const server &tmp);

		/*###########################################
		#				GETTERS						#
		############################################*/
		std::string get_host(void) const 			{return(this->host);};
		std::string get_network_pass(void) const 	{return(this->network_pass);};
		std::string get_network_port(void) const 	{return(this->network_port);};
		std::string get_port(void) const 			{return(this->port);};
		std::string get_password(void) const 		{return(this->password);};
};

std::ostream &operator<<(std::ostream& os, const server &tmp);

#endif
