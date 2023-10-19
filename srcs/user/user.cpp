/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:26:28 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/19 17:03:23 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"
#include "../command/reply.hpp"

user::user( void ) : _fd(0), _is_registered(false), _n_channels(0)
{
  return ;
}

user::user( int fd, std::string hostname ) : _fd(fd), _hostname(hostname), _is_registered(false), _n_channels(0)
{
  return ;
}

user::user( const user & var )
{
  *this = var;
  return ;
}

user::~user( void )
{
  return ;
}

user & user::operator=(const user &tmp) 
{
  this->set_username(tmp.get_username());
  this->set_nick(tmp.get_nick());
  this->set_hostname(tmp.get_hostname());
  this->set_realname(tmp.get_realname());
  this->set_servername(tmp.get_servername());
  this->set_fd(tmp.get_fd());
  this->set_is_registered(tmp.get_is_registered());
  this->set_n_channels(tmp.get_n_channels());
  return (*this);
  
}

std::ostream &operator<<(std::ostream& os, const user &tmp) {
  os << "Username         |     " << tmp.get_username() << std::endl;
	os << "Nickname         |     " << tmp.get_nick() << std::endl;
	os << "Hostname         |     " << tmp.get_hostname() << std::endl;
  os << "Servername       |     " << tmp.get_servername() << std::endl;
	os << "Realname         |     " << tmp.get_realname() << std::endl;
  os << "FD               |     " << tmp.get_fd() << std::endl;
  os << "User registered  |     " << tmp.get_is_registered() << std::endl;
  os << "Member of Nº chn |     " << tmp.get_n_channels() << std::endl;
  return (os);
}

void  user::is_registered(server &svr)
{
  if (!this->_is_registered && !this->get_username().empty() && !this->get_nick().empty())
  {
    this->_is_registered = true;
	  std::string nick = this->get_nick();
	  std::string user = this->get_username();
	  std::string host = this->get_hostname();
	  svr.send_message(RPL_WELCOME(nick, user, host), this->get_fd());
  }
}

void		user::send_to_channel(server &svr, channel *chn, std::string chn_name, std::string msg)
{
  if(chn)
  {
    if (chn->is_user_in_channel(*this))
    {
      for (size_t i = 0; i < chn->get_list_of_members().size(); i++)
        if (chn->get_list_of_members()[i]->get_nick() != this->get_nick())
        	svr.send_message("From " + chn_name + ":\n" + msg + "\n", chn->get_list_of_members()[i]->get_fd());
    }
    else
      svr.send_message(ERR_CANNOTSENDTOCHAN(chn_name), this->get_fd());
  }
  else
    svr.send_message(ERR_NOSUCHNICK(chn_name), this->get_fd());

}

/*###########################################
#				           GETTERS						      #
############################################*/

std::string user::get_username(void) const
{
  return(this->_username);
}

std::string user::get_nick(void) const
{
  return(this->_nick);
}

std::string user::get_hostname(void) const
{
  return(this->_hostname);
}

std::string user::get_servername(void) const
{
  return(this->_servername);
}

std::string user::get_realname(void) const
{
  return(this->_realname);
}

int 		user::get_fd(void) const
{
  return(this->_fd);
}

bool 		user::get_is_registered(void) const
{
  return(this->_is_registered);
}

int 		user::get_n_channels(void) const
{
  return(this->_n_channels);
}

/*###########################################
#				           SETTERS						      #
############################################*/

void		user::set_username(std::string username)
{
  this->_username = username;
}

void		user::set_nick(std::string nick)
{
  this->_nick = nick;
}

void		user::set_hostname(std::string host)
{
  this->_hostname = host;
}

void		user::set_servername(std::string srv)
{
  this->_servername = srv;
}

void		user::set_realname(std::string realname)
{
  this->_realname = realname;
}

void		user::set_fd(int fd)
{
  this->_fd = fd;
}

void		user::set_is_registered(bool is_registered)
{
  this->_is_registered = is_registered;
}

void 		user::set_n_channels(int i)
{
  this->_n_channels = i;
}
