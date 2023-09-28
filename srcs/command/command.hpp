/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:13:47 by crisfern          #+#    #+#             */
/*   Updated: 2023/09/28 11:13:56 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include <iostream>
#include "../server/server.hpp"
#include "../user/user.hpp"

class user;
class server;
namespace cmd
{
	void nick(server &svr, int poll_fd_pos, std::string str);
	void username(server &svr, int poll_fd_pos, std::string str);
	void pong(server &svr, int poll_fd_pos, std::string str);
	void quit(server &svr, int poll_fd_pos, std::string str);
	void privmsg(server &svr, int poll_fd_pos, std::string str);
	void join(server &svr, int poll_fd_pos, std::string str);
	void topic(server &svr, int poll_fd_pos, std::string str);
	void invite(server &svr, int poll_fd_pos, std::string str);
	void kick(server &svr, int poll_fd_pos, std::string str);
	void notice(server &svr, int poll_fd_pos, std::string str);
	void part(server &svr, int poll_fd_pos, std::string str);
	void mode(server &svr, int poll_fd_pos, std::string str);
	void names(server &svr, int poll_fd_pos, std::string str);
	void list(server &svr, int poll_fd_pos, std::string str);
}
#endif
