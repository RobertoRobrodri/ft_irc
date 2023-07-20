#include "server.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <netinet/in.h>

// CONSTRUCTORS

server::server( void )
{

	std::cout << "Server Default constructor called" << std::endl;

}

server::server( std::string network , std::string port , std::string pass ) : _active_fds(1) 
{	
	std::cout << "Server Parameter constructor called" << std::endl;
	std::vector <std::string>seglist = ft_split(network, ':');
	this->data.host 		= seglist[0];
	this->data.network_port = seglist[1];
	this->data.network_pass = seglist[2];
	this->data.port 		= port;
	this->data.pass 		= pass;
	this->init_list_of_cmds();
	this->server_socket = new autosocket(this->data.port, this->data.host);
}

server::server( const server & var ) {
	(void)var;
	std::cout << "Server Copy constructor called" << std::endl;

}

// DESTRUCTOR
server::~server( void ) {

	std::cout << "Server Destructor constructor called" << std::endl;
	delete this;

}

// OVERLOADING
server & server::operator=(const server &tmp)
{
	(void)tmp;
	std::cout << "Server Operator equalizer called" << std::endl;
	return *this;
}

std::ostream &operator<<(std::ostream& os, const server &tmp)
{
	os << "host           |     " << tmp.get_host() << std::endl;
	os << "network pass   |     " << tmp.get_network_pass() << std::endl;
	os << "network port   |     " << tmp.get_network_port() << std::endl;
	os << "password       |     " << tmp.get_password() << std::endl;
	os << "port           |     " << tmp.get_port() << std::endl;
	return (os);
}

// FUNCTIONS
void	server::init_list_of_cmds(void)
{
	this->list_of_cmds.insert(std::pair<std::string, command_function>("NICK", &cmd::nick));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("USER", &cmd::username));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("PONG", &cmd::pong));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("QUIT", &cmd::quit));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("PRIVMSG", &cmd::privmsg));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("JOIN", &cmd::join));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("TOPIC", &cmd::topic));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("INVITE", &cmd::invite));
	this->list_of_cmds.insert(std::pair<std::string, command_function>("KICK", &cmd::kick));
}

void	server::init_pollfd(void)
{
	memset(this->poll_fds, 0, sizeof(this->poll_fds));
	this->poll_fds[0].fd 	   = this->server_socket->fd;
	this->poll_fds[0].events   = POLLIN;
}

bool	server::wait_for_connection(void)
{
	int ret;

	this->init_pollfd();
	while (true)
	{
		ret = poll(this->poll_fds, this->_active_fds, TIMEOUT); //TODO cambiar timeout + check ping clients
		if (ret < 0) {
			perror("Poll error");
			return 1;
		}
		if (ret == 0)
			continue;
		if (this->fd_ready() == 1)
			return 1;
		// ping  users and disconnect inactive
	}
	return 0;
}

bool	server::fd_ready(void)
{
	for (int i = 0; i < this->_active_fds; i++)
	{
		if (this->poll_fds[i].revents == 0)
			continue;
		if (this->poll_fds[i].fd == this->server_socket->fd)
		{
			this->accept_communication();
			return 0;
		}
		else
		{
			this->receive_communication(i);
			return 0;
		}
	}
	return 1;
}

void	server::add_user(int fd, sock_in client_addr)
{
	char ip_address[20];

	this->poll_fds[this->_active_fds].fd = fd;
	this->poll_fds[this->_active_fds].events = POLLIN;
	this->_active_fds++;
	user 	new_user(fd, inet_ntop(AF_INET, &(client_addr.sin_addr), ip_address, sizeof(ip_address)));
	this->list_of_users.insert(std::pair<int, user>(fd, new_user));
}

bool	server::accept_communication(void)
{
	int 	fd;
	sock_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);

	fd = accept(this->server_socket->fd, (sock_addr*)&client_addr, &client_addr_size);
	if (fd < 0)
    {
        if (errno != EWOULDBLOCK)
          perror("  accept() failed");
    	return 1;
    }
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror(" FCNTL failed");
		return 1;
	}
	std::cout << YELLOW << "Accepted communication from: " << fd << RESET << std::endl;
	this->add_user(fd, client_addr);
	return 0;
}

bool	server::receive_communication(int poll_fd_pos)
{
	char buffer[MSG_SIZE];
	int len;

	std::cout << "Message received" << std::endl;
	memset(buffer, 0, MSG_SIZE); //Iniciar buffer con ceros porque mete mierda
	len = recv(this->poll_fds[poll_fd_pos].fd, buffer, sizeof(buffer), 0);
	std::cout << GREEN << buffer << RESET <<std::endl;
	if (len < 0)
    {
		if (errno != EWOULDBLOCK)
			perror("  recv() failed");
        exit(1);
    }
    if (len == 0)
    {
		std::cout << "  Connection closed" << std::endl;
		// Close fd >> Delete fd from poll >> Delete user from list_of_users
		this->delete_user(poll_fd_pos);
		return 0;
    }
	buffer[len-1] = 0; //El intro lo ponemos a cero
	if (buffer[0] != 0)
	{
		std::map<std::string, std::string> commands = this->parse_message(buffer);
		this->execute_commands(poll_fd_pos, commands);
	}
	return 0;
}

bool	server::send_message(std::string msg, int fd)
{
	int len = send(fd, msg.c_str(), msg.length(), 0);
	if (len < 0)
    {
		perror("  send() failed");
		return 1;
    }
	return 0;
}

void	server::delete_user(int poll_fd_pos)
{
	std::cout << RED << "Deleted user: " << this->poll_fds[poll_fd_pos].fd
		<< RESET << std::endl;
	close(this->poll_fds[poll_fd_pos].fd);
	this->list_of_users.erase(this->poll_fds[poll_fd_pos].fd);
	for (int count = poll_fd_pos; count <= this->_active_fds - 1; count++)
		this->poll_fds[count] = this->poll_fds[count + 1];
	this->poll_fds[this->_active_fds - 1].fd = 0;
	this->poll_fds[this->_active_fds - 1].events = 0;
	this->_active_fds--;
	//this->poll_fds[poll_fd_pos].fd = -1;
}

// Separa la cadena en COMANDO + MSG, donde mensaje es todo lo demás que es parseado de forma distinta por cada comando
std::map<std::string, std::string> server::parse_message(std::string msg)
{
	// Este split es por culpa de irssi, que lanza todos los comandos NICK y USER en una sola linea
	// No deberia afectar a los usuarios que lanzan comandos de uno en uno
	std::vector<std::string> seglist = ft_split(msg, '\n');
	std::vector<std::string>::iterator v_it;
	std::map<std::string, std::string> commands;

	for (v_it = seglist.begin(); v_it != seglist.end(); v_it++)
	{
		int ind = v_it->find(" ");
		std::string cmd = v_it->substr(0, ind);
		std::string args= v_it->substr(ind + 1);
		commands.insert(std::pair<std::string, std::string>(cmd, args));
	}
	return commands;
}

void	server::execute_commands(int poll_fd_pos, std::map<std::string, std::string> commands)
{
	std::map<std::string, std::string>::iterator it;

	for (it = commands.begin(); it != commands.end(); it++)
	{
		if (this->list_of_cmds[it->first])
			this->list_of_cmds[it->first](*this, poll_fd_pos, it->second);
	}
}

void	server::create_channel(user &usr, std::string name)
{
	channel cnn(name);

	cnn.add_member(usr);
	this->list_of_channels.insert(std::pair<std::string, channel>(name, cnn));
	std::cout << name << " channel created!" << std::endl;
	std::cout << cnn << std::endl;
}

// Maybe make a template????
user *server::get_user_from_nick(std::string nick)
{
	std::map<int, user>::iterator it;

	for (it = this->list_of_users.begin(); it != this->list_of_users.end(); it++)
	{
		if (it->second.get_nick().compare(nick) == 0)
			return &(it->second);
	}
	return NULL;
}

channel *server::get_channel_from_name(std::string name)
{
	std::map<std::string, channel>::iterator it;

	for (it = this->list_of_channels.begin(); it != this->list_of_channels.end(); it++)
	{
		if (it->second.get_name().compare(name) == 0)
			return &(it->second);
	}
	return NULL;
}

user& server::get_user(int i) {
	return(this->list_of_users.find(i)->second);
}

pollfd&	server::get_pollfd(int i) {
	return (this->poll_fds[i]);
}

// TESTS
void	test_check_data_correct()
{
	char *program = "./ircserv";
	char *arg1 = "";
	char *arg2 = "";
	char *arg3 = "";

	char *argv[] = {program, arg1, arg2, arg3, NULL};

	std::cout << "Test check_data_correct\n";
	std::cout << "==========================\n";
	std::cout << std::boolalpha;

	std::cout << "Params: \" \" \" \" \" \"\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";
	
	argv[1] = "a";
	argv[2] = "b";
	argv[3] = "c";
	std::cout << "Params: a b c\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";
	
	argv[1] = "0:0:a";
	argv[2] = "0";
	argv[3] = "a";
	std::cout << "Params: 0:0:a 0 a\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "0.0.0.:6776:pass";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 0.0.0.:6776:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "127.0.-1.1:6776:pass";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 127.0.-1.1:6776:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "127.0.0.1:-9:pass";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 127.0.0.1:-9:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "127.0.0.1::";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 127.0.0.1:: 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "127.0.0.1";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 127.0.0.1 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "127.0.0.1::6776:pass";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 127.0.0.1::6776:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";

	argv[1] = "127.0.0.1:6776:pass";
	argv[2] = "6776";
	argv[3] = "pass";
	std::cout << "Params: 127.0.0.1:6776:pass 6776 pass\n";
	std::cout << "Output: " << check_data_correct(argv) << "\n\n";
}

server	*test_server_construction(char *arg1, char *arg2, char *arg3)
{
	std::cout << "Test server construction\n";
	std::cout << "==========================\n";
	server *serv;

	serv = new server(arg1, arg2, arg3);
	std::cout << *serv << std::endl;

	std::map<std::string, command_function>::iterator it;

    std::cout << "Commands:" << std::endl;
	for (it = serv->list_of_cmds.begin(); it != serv->list_of_cmds.end(); it++)
	{
    	std::cout << it->first << std::endl;
	}
    std::cout << std::endl;

    std::cout << "Socket:" << std::endl;
	std::cout << "File descriptor " << serv->server_socket->fd << std::endl
		<< "Sock_in \n" 
		<< " - sin_family '\\x0" << (int)serv->server_socket->addr.sin_family << "'" << std::endl
		<< " - sin_port " << serv->server_socket->addr.sin_port << std::endl
		<< " - sin_addr " << serv->server_socket->addr.sin_addr.s_addr << std::endl
		<< " - sin_zero [ ";
   	for (int i = 0; i < 8; i++)
	{
		std::cout << (int)serv->server_socket->addr.sin_zero[i] << " ";
	}
	std::cout << "]\n\n";

	return serv;
}

void	test_add_user(server *serv, int fd, char *url, int port)
{
	std::cout << "Test add user" << std::endl;
	std::cout << "==================================================" << std::endl;
	
	struct sockaddr_in myaddr;

	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	inet_aton(url, &myaddr.sin_addr);

	serv->add_user(fd, myaddr);

	std::cout << "New socket: " << fd << std::endl;
	std::cout << "Active clients: " << serv->_active_fds << std::endl;
	std::cout << "New user: " << std::endl;
	std::cout << serv->list_of_users[fd] << std::endl;
}

void	test_delete_user(server *serv, int fd_pos)
{
	std::cout << "Test delete user" << std::endl;
	std::cout << "==================================================" << std::endl;
	std::cout << "Active clients: " << serv->_active_fds << std::endl;
	std::cout << "List of users:" << std::endl;
	for (unsigned int i = 0; i < serv->list_of_users.size(); i++)
	{
		if (serv->list_of_users[i].get_fd() > 0)
			std::cout << serv->list_of_users[i] << std::endl << std::endl;
	}
	std::cout << "Poll fd:" << std::endl;
	for (int i = 0; i < serv->_active_fds; i++)
	{
		std::cout << i << " - " << "fd " << serv->poll_fds[i].fd << ", "
			<< "events " << serv->poll_fds[i].events << std::endl;
	}
	
	serv->delete_user(fd_pos);
	
	std::cout << "Active clients: " << serv->_active_fds << std::endl;
	std::cout << "List of users:" << std::endl;
	for (unsigned int i = 0; i < serv->list_of_users.size(); i++)
	{
		if (serv->list_of_users[i].get_fd() > 0)
			std::cout << serv->list_of_users[i] << std::endl << std::endl;
	}
	std::cout << "Poll fd:" << std::endl;
	for (int i = 0; i < serv->_active_fds; i++)
	{
		std::cout << i << " - " << "fd " << serv->poll_fds[i].fd << ", "
			<< "events " << serv->poll_fds[i].events << std::endl;
	}
}

void	test_parse_message(server *serv, std::string msg)
{
	std::cout << "Raw message: | " << msg << " |\n";
	std::map<std::string, std::string> commands;
	commands = serv->parse_message(msg);

	std::cout << "Parsed message:\n";
	std::map<std::string, std::string>::iterator it;
	for (it = commands.begin(); it != commands.end(); it++)
		std::cout << "Command: |" << it->first << "| " << "Arguments: |" << it->second << "|\n";
}

void	test_connection(server *serv)
{
	std::cout << "CONNECT\n" << "Open a new terminal and type nc -v 127.0.0.1 6776 to test new connection.\n"
		<< serv->wait_for_connection() << std::endl;
}
