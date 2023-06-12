#include "autosocket.hpp"

autosocket::autosocket( void ) {

  std::cout << "Autosocket Default constructor called" << std::endl;
  return ;
}

autosocket::autosocket( std::string port, std::string host ) {
  std::cout << "Autosocket Parameter constructor called" << std::endl;
  this->init_socket_struct(port, host);
  try {
    if (this->server_listening() == 0)
      throw std::runtime_error("Server not listening");
  }
  catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
    exit (1);
	}
  return ;
}

autosocket::autosocket( const autosocket & var ) {
  
  std::cout << "Autosocket Copy constructor called" << std::endl;
  *this = var;
  return ;
  
}

autosocket::~autosocket( void ) {
  
  std::cout << "Autosocket Destructor called" << std::endl;
  delete this;
  return ;

}

// Overloading
autosocket & autosocket::operator=(const autosocket &tmp) {

  (void) tmp;
  std::cout << "Autosocket Operator equalizer called" << std::endl;
  return (*this);
  
}

std::ostream &operator<<(std::ostream& os, const autosocket &tmp) {

  (void) tmp;
	os << std::endl << "Autosocket Operator output called" << std::endl;
	return (os);
  
}

// Functions

void	autosocket::init_socket_struct(std::string port, std::string host)
{
// Init struct that the socket needs
//  IPV4 addresses
	this->addr.sin_family			 = AF_INET;
//  Convert our port to a network address (host to network)
	this->addr.sin_port				 = htons(atoi(port.c_str()));
//  Our address as integer
	this->addr.sin_addr.s_addr = inet_addr(host.c_str());
}

bool	autosocket::server_listening(void)
{
	int		opt = 1;

	if ((this->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return 0;
	fcntl(this->fd, F_SETFL, O_NONBLOCK);
	std::cout << "Init socket" << std::endl;
	// No sé que opciones tendremos que habilitar pero vamos a tener que usarlo
	if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		perror("Bad socket ");
		return 0;
	}
	// Asigna un nombre al socket; Asigna la info de address al socket
	if (bind(this->fd, (const sock_addr*)&this->addr, sizeof(this->addr)) == -1)
	{
		perror("Error binding ");
		return 0;
	}
	// Tenemos que definir un max_size para la cola
	if (listen(this->fd, MAX_CLIENTS) == -1)
	{
		perror("Can't hear you");
		return 0;
	}
	return 1;
}