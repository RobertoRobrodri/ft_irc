#include "channel.hpp"

channel::channel( void ) : _user_limit(0) {
  return ;
}

channel::channel( std::string str ) :  _name(str), _user_limit(0) {
  return ;
}

channel::channel( std::string c_name, std::string password ) :  _name(c_name), _password(password), _user_limit(0) {
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
  /*
	std::cout << "Join successful! " << std::endl;
  std::cout << usr << std::endl;
  */
  std::string channel = this->get_name();
  
  std::string topic = this->get_topic();
  if (topic.empty())
  	server::send_message(RPL_NOTOPIC(channel), usr.get_fd());
  else
  	server::send_message(RPL_TOPIC(channel, topic), usr.get_fd());
  
  	std::string members;
	std::vector<user>::iterator it;
  	for (it = this->list_of_members.begin(); it != this->list_of_members.end(); it++)
		members += (it->get_nick() + " ");
  	for (it = this->list_of_members.begin(); it != this->list_of_members.end(); it++)
  		server::send_message(RPL_NAMREPLY(channel, members), usr.get_fd());
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

void 	channel::parse_mode_flag(user &usr, std::string &modes, std::vector<std::string> mode_params, server &srv)
{
	bool sign = 0;
	size_t j = 0;
  	std::string tmp;

	for (size_t i = 0; i < modes.size(); i++)
	{
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
        		if (mode_params.empty())
          			break ;
				std::string nick = mode_params[j++];
				user *new_op = srv.get_user_from_nick(nick);
				if (!new_op)
					break;
				set_user_operator(*new_op, sign);
        		break;
			}
			case 'i':	// Invite only
      		case 't':	// topic settable by channel operator only flag
      		case 'p':	// set channel to private
      		case 's':	// set channel to secret
      		{
      			if (sign == true)
      			{
					if (this->get_mode().find_first_of("itps") != std::string::npos)
						break ;
					tmp = this->get_mode();
      				tmp.push_back(modes[i]);
      				this->set_mode(tmp);
				}
				else
				{
					size_t pos = this->get_mode().find(modes[i]);
					if (pos != std::string::npos)
					{
						tmp = this->get_mode().erase(pos, 1);
						this->set_mode(tmp);
					}
				}
        		break;
      		}
			case 'l':	// set user limit
			{
				if (sign == true)
				{
          			if (mode_params.empty())
            			break ;
					this->set_user_limit(atoi(mode_params[j++].c_str()));
					tmp = this->get_mode();
					if (tmp.find('l') != std::string::npos)
						break ;
          			tmp.push_back(modes[i]);
          			this->set_mode(tmp);
				}
				else
				{
					this->set_user_limit(0);
					size_t pos = this->get_mode().find('l');
					if (pos != std::string::npos)
					{
						tmp = this->get_mode().erase(pos);
						this->set_mode(tmp);
					}
				}
        		break;
			}
			case 'k':	// set password for channel
			{
				if (sign == true)
				{
          			if (mode_params.empty())
            			break ;
					this->set_password(mode_params[j++]);
					tmp = this->get_mode();
					if (tmp.find('k') != std::string::npos)
						break ;
          			tmp.push_back(modes[i]);
          			this->set_mode(tmp);
        		}
				else
				{
					this->set_password("");
					size_t pos = this->get_mode().find('k');
					if (pos != std::string::npos)
					{
						tmp = this->get_mode().erase(pos);
						this->set_mode(tmp);
					}
				}
        		break ;
			}
      		default :
      		{
				std::string mode (&modes[i]);
				srv.send_message(ERR_UNKNOWNMODE(mode), usr.get_fd());
        		return;
      		}
		}
	}
		std::string params;
		for (std::vector<std::string>::iterator i = mode_params.begin(); i != mode_params.end(); ++i)
    		params += *i;
		for (std::vector<user>::iterator it = this->list_of_members.begin(); it != this->list_of_members.end(); it++)
			srv.send_message(RPL_CHANNELMODEIS(this->_name, modes, params), it->get_fd());
}
