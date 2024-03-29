#include "user.hpp"
#include "../command/reply.hpp"

user::user( void ) : _fd(0), _is_registered(false), _n_channels(0), _op(false) {
  return ;
}

user::user( int fd, std::string hostname ) : _fd(fd), _hostname(hostname), _is_registered(false), _n_channels(0), _op(false) {
  return ;
}

user::user( const user & var ) {
  *this = var;
  return ;
}

user::~user( void ) {
  return ;

}

user & user::operator=(const user &tmp) {
  this->set_username(tmp.get_username());
  this->set_nick(tmp.get_nick());
  this->set_hostname(tmp.get_hostname());
  this->set_realname(tmp.get_realname());
  this->set_servername(tmp.get_servername());
  this->set_fd(tmp.get_fd());
  this->set_is_registered(tmp.get_is_registered());
  this->set_n_channels(tmp.get_n_channels());
  this->set_op(tmp.get_op());
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
	os << "Is operator      |     " << tmp.get_op() << std::endl; 
  return (os);
}

void  user::is_registered(server &svr)
{
  std::cout << *this << std::endl;
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
    if (chn->is_user_in_channel(*this))//TODO habria que comprobar o de los modos también
    {
      for (size_t i = 0; i < chn->get_list_of_members().size(); i++)
        if (chn->get_list_of_members()[i].get_nick() != this->get_nick())
        	svr.send_message("From " + chn_name + ":\n" + msg + "\n", chn->get_list_of_members()[i].get_fd());
    }
    else
      svr.send_message(ERR_CANNOTSENDTOCHAN(chn_name), this->get_fd());
  }
  else
    svr.send_message(ERR_NOSUCHNICK(chn_name), this->get_fd());

}
