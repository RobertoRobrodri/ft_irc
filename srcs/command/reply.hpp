// Todos los defines con los mensajes con los que tiene que responder el servidor
#ifndef REPLY_HPP
#define REPLY_HPP

# define RPL_WELCOME(nick, user, host) ": 001 :Welcome to the Internet Relay Network " + nick + "!\n" + user + "@" + host + "\r\n"
# define ERR_ALREADYREGISTERED ": 462 :You may not reregister\r\n"
# define ERR_NEEDMOREPARAMS(command) ": 461 " + command + " :Not enough parameters\r\n"

#endif
