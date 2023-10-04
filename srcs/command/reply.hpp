/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:19:28 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/04 12:52:15 by mzomeno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// All messages that server needs to answer with
#ifndef REPLY_HPP
#define REPLY_HPP

# define RPL_WELCOME(nick, user, host) ": 001 :Welcome to the Internet Relay Network " + nick + "!\n" + user + "@" + host + "\r\n"
# define RPL_AWAY(nick, msg) ": 301 " + nick + " :" + msg + "\r\n"
# define RPL_LISTSTART ": 321 Channel :Users Name\r\n"
# define RPL_LIST(channel, private, topic) ": 322 " + channel + " " + private + ": " + topic + "\r\n"
# define RPL_LISTEND ": 323 :End of /LIST\r\n"
# define RPL_CHANNELMODEIS(channel, mode, params) ": 324 : " + channel + " " + mode + " " + params + "\r\n"
# define RPL_NOTOPIC(channel) ": 331 " + channel + " :No topic is set\r\n"
# define RPL_TOPIC(channel, topic) ": 332 " + channel + " :" + topic + "\r\n"
# define RPL_INVITING(channel, nick) ": 341 " + channel + " " + nick + "\r\n"
# define RPL_NAMREPLY(channel, members) ": 353 " + channel + " :" + members + "\r\n"
# define RPL_ENDOFNAMES(channel) ": 366 " + channel + " :End of /NAMES list\r\n"
# define ERR_NOSUCHNICK(rcv) ": 401 " + rcv + " :No such nick/channel\r\n"
# define ERR_NOSUCHCHANNEL(channel) ": 403 " + channel + " :No such channel\r\n"
# define ERR_CANNOTSENDTOCHAN(channel) ": 404 " + channel + " :Cannot send to channel\r\n"
# define ERR_TOOMANYCHANNELS(channel) ": 405 " + channel + " :You have joined too many channels\r\n"
# define ERR_TOOMANYTARGETS(target) ": 407 " + target + ":Duplicate recipients. No message delivered\r\n"
# define ERR_NORECIPIENT(command) ": 411 :No recipient given (" + command + ")\r\n"
# define ERR_NOTEXTTOSEND ": 412: No text to send\r\n"
# define ERR_UNKNOWNCOMMAND(command) ": 421 " + command + ":Unknown command\r\n"
# define ERR_NONICKNAMEGIVEN ": 431 :No nickname given\r\n"
# define ERR_ERRONEUSNICKNAME(nick) ": 432 " + nick + " :Erroneus nickname\r\n"
# define ERR_NICKNAMEINUSE(nick) ": 433 " + nick + " :Nickname is already in use\r\n"
# define ERR_NOTONCHANNEL(channel) ": 442 " + channel + " :You're not on that channel\r\n"
# define ERR_USERONCHANNEL(user, channel) ": 443 " + user + " " + channel + ":is already on channel\r\n"
# define ERR_NOTREGISTERED ": 451 :You have not registered\r\n"
# define ERR_NICKNOTREGISTERED(nick) ": 452 :" + nick + " is not registered\r\n"
# define ERR_NEEDMOREPARAMS(command) ": 461 " + command + " :Not enough parameters\r\n"
# define ERR_ALREADYREGISTERED ": 462 :You may not reregister\r\n"
# define ERR_CHANNELISFULL(channel) ": 471 " + channel + " :Cannot join channel (+l)\r\n"
# define ERR_UNKNOWNMODE(mode) ": 472 : " + mode + " is unknown mode char to me\r\n"
# define ERR_INVITEONLYCHAN(channel) ": 473 " + channel + " :Cannot join channel (+i)\r\n"
# define ERR_BADCHANNELKEY(channel) ": 475 " + channel + " :Cannot join channel (+k)\r\n"
# define ERR_CHANOPRIVSNEEDED(channel) ": 482 " + channel + " :You're not channel operator\r\n"

#endif
