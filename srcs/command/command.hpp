#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include <iostream>
#include "../server/server.hpp"

class	command {

	protected:

		std::string 	_cmd;

	public:

		command( void );
		command(std::string cmd);
		virtual ~command 			( void );
		virtual void	execute(int fd, user user, std::string cmd) = 0;
};

class	nick : public command {

	public:

		nick( void );
		~nick( void );
		void			execute(int fd, user user, std::string cmd);
};

#endif
