# Compilation
[ ] Compile source files and generate program with c++98 flag
[ ] No relink
[ ] Rules $(NAME), all, clean, fclean and re in Makefile

# Restrictions
[ ] No external libraries
[ ] C libraries and functions only used when needed
[ ] No forks

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

                       1--\
                           A        D---4
                       2--/ \      /
                             B----C
                            /      \
                           3        E

   Servers: A, B, C, D, E         Clients: 1, 2, 3, 4


## One-To-One Communication
### Ref: www.rfc-editor.org/rfc/rfc2810#section-5.1

The path of a message being delivered is the shortest path between any two points on the network.

   [ ] Example 1: A message between clients 1 and 2 is only seen by server
       A, which sends it straight to client 2.

   [ ] Example 2: A message between clients 1 and 3 is seen by servers A &
       B, and client 3.  No other clients or servers are allowed see the
       message.

   [ ] Example 3: A message between clients 2 and 4 is seen by servers A, B,
       C & D and client 4 only.


## Channel Communication
### Ref: www.rfc-editor.org/rfc/rfc2810#section-5.2.1

   Each server is fans the original message so that it will reach all the recipients.

   [ ] Example 4: 1 client. Messages to the channel go to the server 
   		and then nowhere else.

   [ ] Example 5: 2 clients. All messages traverse a path as if
       they were private messages between the two clients outside a
       channel.

   [ ] Example 6: Clients 1, 2 and 3 in a channel.  Messages to the are 
   	   sent to all clients and only those servers which must be
	   traversed to relay the message.  If client 1 sends a message,
	   it goes back to client 2 and then via server B to client 3.

