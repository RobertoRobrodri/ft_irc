/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:16:27 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/19 19:09:11 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "reply.hpp"

// Parameters: <nickname> [ <hopcount> ]
//
//   is used to give user a nickname or change the previous one.
//
//   The <hopcount> parameter must be ignored.
//
//      Numeric Replies:
//
//           ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
//           ERR_NICKNAMEINUSE

bool invalid_nick(std::string str)
{
  if ((str.find_first_of(" .:,?!@*&#$") != std::string::npos) || (str.length() > 9))
    return true;
  return false;
}

void cmd::nick(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  //get reference of the user
  user &usr = svr.get_user(pollfd.fd);
  user *nick;
  if (str == "")
  {
	  svr.send_message(ERR_NONICKNAMEGIVEN, usr.get_fd());
	  return ;
  }
  if (invalid_nick(str))
  {
	  svr.send_message(ERR_ERRONEUSNICKNAME(str), usr.get_fd());
	  return ;
  }
  if ((nick = svr.get_user_from_nick(str)) != 0)
  {
    if (nick->get_nick().compare(usr.get_nick()) == 0)
      return ;
	  svr.send_message(ERR_NICKNAMEINUSE(str), usr.get_fd());
	  return ;
  }
  usr.set_nick(str);
  usr.is_registered(svr);
  std::cout << usr << std::endl;
}
