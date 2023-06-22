# Server

[ ] Identified by unique name shorter than 63 characters
[ ] Server must know each client's unique identifier
[ ] Server must know nickname, surname and host of each user

====

# Clients

## Client Location
### Ref: www.rfc-editor.org/rfc/rfc2810#section-4.1

   [ ] Check client location in server

   Upon connecting to a server, a client registers using a label which
   is then used by other servers and clients to know where the client is
   located.  Servers are responsible for keeping track of all the labels
   being used.

====

# Communication

[ ] Create server-client network

                       client_1--\
                                server 
                       client_2--/ 

   Servers: A        Clients: 1, 2


## One-To-One Communication
### Ref: www.rfc-editor.org/rfc/rfc2810#section-5.1

The path of a message being delivered is the shortest path between any two points on the network.

   [ ] Example 1: A message between clients 1 and 2 is only seen by server
       A, which sends it straight to client 2.


## Channel Communication
### Ref: www.rfc-editor.org/rfc/rfc2810#section-5.2.1

   Each server fans the original message so that it will reach all the recipients.

   [ ] 1 client. Messages to the channel go to the server and then nowhere else.

   [ ] Many clients. All messages traverse a path as if they were private messages             between the two clients outside a channel.

## Messages

   Each IRC message may consist of up to three main parts: the prefix
   (OPTIONAL), the command, and the command parameters (maximum of
   fifteen (15)).  The prefix, command, and all parameters are separated
   by one ASCII space character (0x20) each.

   IRC messages are always lines of characters terminated with a CR-LF
   (Carriage Return - Line Feed) pair, and these messages SHALL NOT
   exceed 512 characters in length, counting all characters including
   the trailing CR-LF.

   message    =  [ ":" prefix SPACE ] command [ params ] crlf

   [ ] Check valid message

   ### Prefix

   Used by servers to indicate the origin of the message. By default, it is assumed to
   have originated from the connection from which it was received from.

   Indicated with a colon (:) which MUST be the first character of the message itself.
   There MUST be NO gap (whitespace) between the colon and the prefix.

   prefix     =  servername / ( nickname [ [ "!" user ] "@" host ] )

   ### Command

   MUST either be a valid IRC command or a 3 digit number represented in ASCII text.
   
   [ ] Check valid command

   #### PASS command

   Command: PASS
   Parameters: <password>
   
   Not required for connection. Only valid if it is set before sending the NICK/USER
   combination to the server.

   [ ] Implement
   [ ] If no password -> ERR_NEEDMOREPARAMSN
   [ ] If already connected -> ERR_ALREADYREGISTRED

   #### NICK command

   Command: NICK
   Parameters: <nickname>

   [ ] Implement
   [ ] If no nickname -> ERR_NONICKNAMEGIVEN
   [ ] If nickname not unique -> ERR_NONICKNAMEGIVEN
   [ ] If bad nickname -> ERR_NONICKNAMEGIVEN
      Rules for nickname: ( letter / special ) *8( letter / digit / special / "-" )
   [ ] If -> ERR_UNAVAILRESOURCE
   [ ] If -> ERR_NICKCOLLISION
   [ ] If -> ERR_RESTRICTED

   ### Replies

   [ ] If target of the message could not be found, the reply ERR_NOSUCHSERVER is              returned. The server MUST NOT send any other replies after this error for that         command.
   [ ] In the case of incorrect messages which use parameter lists with comma as an
      item separator, a reply MUST be sent for each item.

====

# Users

[ ] Identified by unique nickname shorter than 9 characters
[ ] Hostname has a maximum length of 63 characters
