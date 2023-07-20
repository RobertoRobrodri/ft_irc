MAIN
|
CHECK_DATA_CORRECTLY
		|
		SERVER()
			|
			AUTOSOCKET()
				|
				INIT_SOCKET_STRUCT
					|
					SERVER_LISTENING
						|
						WAIT_FOR_CONNECTION
							|
							INIT_POLLFD
								|
								FD_READY
								/		\
			ACCEPT_COMMUNICATION		RECEIVE_COMMUNICATION
			|							/					\
			ADD_USER			DELETE_USER					PARSE_MESSAGE
															|
															EXECUTE_COMMAND


COMMANDS:

USER
|


NICK
|


JOIN
|


PONG
|


QUIT
|


PRIVMSG
|


TOPIC
|


INVITE
|


KICK
|


