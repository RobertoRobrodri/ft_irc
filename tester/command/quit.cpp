#include "command.hpp"
#include "reply.hpp"

void  cmd::quit(server &svr, int poll_fd_pos, std::string str) {
  poll_fd pollfd = svr.get_pollfd(poll_fd_pos);
  user &usr = svr.get_user(pollfd.fd);
  // BUG si no pasamos parámetros str == QUIT
  // Qué pasa si quiero que mi mensaje sea QUIT?
  // Este comportamiento se repite para cada comando
  if (str.empty())
	str += "Cierra al salir (Mensaje por defecto del server)";
  str += "\r\n";
  svr.send_message(str, usr.get_fd());
  svr.delete_user(poll_fd_pos);
}