#include "channel.hpp"

channel::channel( void ) : _user_limit(0) {

  std::cout << "Default channel constructor called" << std::endl;
  return ;
}

channel::channel( std::string str ) : _name(str), _user_limit(0) {
  return ;
}

channel::channel( std::string c_name, std::string password ) : _name(c_name), _password(password), _user_limit(0) {
  return ;
}

channel::channel( const channel & var ) {
  
  *this = var;
  return ;
  
}

channel::~channel( void ) {
  
  return ;

}

channel & channel::operator=(const channel &tmp) {
  this->_name = tmp.get_name();
  this->_topic = tmp.get_topic();
  this->list_of_members = tmp.list_of_members;
  this->_password = tmp.get_password();
  this->_mode = tmp.get_mode();
  this->_user_limit = tmp.get_user_limit();
  return (*this);
}

std::ostream &operator<<(std::ostream& os, const channel &tmp) {
  std::vector<user>::iterator it;
	std::vector<user> lst = tmp.get_list_of_members();

	os << "Channel name: " << tmp.get_name() << std::endl;
	os << "Channel topic: " << tmp.get_topic() << std::endl;
  os << "Channel password: "<< tmp.get_password() << std::endl;
  os << "Channel mode: " << tmp.get_mode() << std::endl;
  os << "User limit if set: " << tmp.get_user_limit() << std::endl;
  os << "List of members: " << std::endl;
	for (it = lst.begin(); it != lst.end(); it++)
    	os << it->get_nick() << std::endl;
	return (os);
}

void	channel::add_member(user &usr)
{
  usr.set_n_channels(usr.get_n_channels() + 1);
	this->list_of_members.push_back(usr);
  std::string members;
  std::cout << "Join successful! " << std::endl;
  std::cout << usr << std::endl;
  //RPL_TOPIC
  server::send_message(":332 " + this->get_name() + " :" + this->get_topic() + "\r\n", usr.get_fd());
  //RPL_NAMREPLY
  for (std::vector<user>::iterator it = this->list_of_members.begin(); it != this->list_of_members.end(); it++)
    members += (it->get_nick() + " ");
  server::send_message(":353 " + members + "\r\n", usr.get_fd());
}

void	channel::rmv_member(user &usr)
{
  std::vector<user>::iterator it;
  for (it = this->list_of_members.begin(); it != this->list_of_members.end(); it++)
  {
    if (it->get_nick() == usr.get_nick())
    {
	    this->list_of_members.erase(it);
      usr.set_n_channels(usr.get_n_channels() - 1);
      return ;
    }
  }
}

bool	channel::is_user_in_channel(const user &usr)
{
  for (std::vector<user>::iterator it = this->list_of_members.begin(); it != this->list_of_members.end(); it++)
  {
    if (it->get_nick() == usr.get_nick())
      return true;
  }
  return false;
}

bool channel::is_user_operator(const user &usr)
{
  for (std::vector<user>::iterator it = this->list_of_members.begin(); it != this->list_of_members.end(); it++)
  {
    if (it->get_nick() == usr.get_nick())
    {
      return (it->get_op());
    }
  }
  return false;
}

void	channel::set_user_operator(const user &usr, const bool &flag)
{
  for (std::vector<user>::iterator it = this->list_of_members.begin(); it != this->list_of_members.end(); it++)
  {
    if (it->get_nick() == usr.get_nick())
    {
      it->set_op(flag);
      return;
    }
  }
}

void 	channel::parse_mode_flag(std::string &modes, std::vector<std::string> mode_params, server &svr)
{
	bool sign = 0;
	size_t j = 0;
  std::string tmp;

  std::cout << modes << std::endl;
	for (size_t i = 0; i < modes.size(); i++)
	{
    std::cout << mode_params[j] << std::endl;
		switch(modes[i])
		{
			case '+':
			{
				sign = true;
        break;
			}
			case '-':
			{
				sign = false;
        break;
			}
			case 'o':
			{
				user *usr = svr.get_user_from_nick(mode_params[j++]);
				if (!usr)
					break;
				if (this->is_user_in_channel(*usr))
					this->set_user_operator(*usr, sign);
        break;
			}
			// Invite only
			case 'i':
			// topic settable by channel operator only flag
			case 't':
			{
				if (sign == true)
        {
					tmp = this->get_mode();
          tmp.push_back(modes[i]);
          this->set_mode(tmp);
        }
				else
				{
					size_t pos = this->get_mode().find(modes[i]);
					tmp = this->get_mode().erase(pos);
          this->set_mode(tmp);
				}
        break;
			}
			// set user limit
			case 'l':
			{
				if (sign == true)
				{
					this->set_user_limit(atoi(mode_params[j++].c_str()));
					tmp = this->get_mode();
          tmp.push_back(modes[i]);
          this->set_mode(tmp);
				}
				else
				{
					this->set_user_limit(0);
					size_t pos = this->get_mode().find('l');
					tmp = this->get_mode().erase(pos);
          this->set_mode(tmp);
				}
        break;
			}
			// set password for channel
			case 'k':
			{
				if (sign == true)
				{
					this->set_password(mode_params[j++]);
					tmp = this->get_mode();
          tmp.push_back(modes[i]);
          this->set_mode(tmp);
        }
				else
				{
					this->set_password("");
					size_t pos = this->get_mode().find('k');
					tmp = this->get_mode().erase(pos);
          this->set_mode(tmp);
				}
        break ;
			}
      default :
      {
        std::cout << "No existe el modo: " << modes[i] << std::endl;
        break;
      }
		}
	}
}
