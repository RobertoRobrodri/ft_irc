# COLORS #
#
GREEN 	= @echo "\033[0;32m"
BLUE 	= @echo "\033[0;34m"
PURPLE 	= @echo "\033[0;35m"
CYAN 	= @echo "\033[0;36m"
RESET 	= "\033[1;0m"

# VARIABLES #
#
NAME 		= ircserv
CC 			= c++
INCLUDE 	= -std=c++98
CXXFLAGS 	= -Wall -Wextra -g -fsanitize=address -Wno-c++11-compat-deprecated-writable-strings #-Werror
TEST		= ircTester
MOCK		= mock


# PATHS #
#
SRC_PATH    	= srcs

SUBFILE1_PATH   = server
SUBFILE2_PATH   = autosocket
SUBFILE3_PATH   = parser
SUBFILE4_PATH   = user
SUBFILE5_PATH   = command
SUBFILE6_PATH   = channel

OBJ_PATH    	= objects
TEST_PATH		= tester


# SOURCES #
#
SUBFILE1_SRC = server.cpp
SUBFILE2_SRC = autosocket.cpp
SUBFILE3_SRC = parser.cpp
SUBFILE4_SRC = user.cpp
SUBFILE5_SRC = invite.cpp \
			   join.cpp \
			   kick.cpp \
			   nick.cpp \
			   pong.cpp \
			   privmsg.cpp \
			   quit.cpp \
			   topic.cpp \
			   user.cpp \
			   part.cpp \
#			   notice.cpp \
			   mode.cpp
SUBFILE6_SRC = channel.cpp

SRC =	$(addprefix $(SUBFILE1_PATH)/, $(SUBFILE1_SRC)) \
		$(addprefix $(SUBFILE2_PATH)/, $(SUBFILE2_SRC)) \
		$(addprefix $(SUBFILE3_PATH)/, $(SUBFILE3_SRC)) \
		$(addprefix $(SUBFILE4_PATH)/, $(SUBFILE4_SRC)) \
		$(addprefix $(SUBFILE5_PATH)/, $(SUBFILE5_SRC)) \
		$(addprefix $(SUBFILE6_PATH)/, $(SUBFILE6_SRC)) \

SERVER_MAIN = objects/main.o
TESTER_MAIN = tester/main.o
MOCK_MAIN = tester/mock_main.o

# RULES #
#
all: $(NAME)

test: $(TEST)

mock: $(MOCK)

SRCS = $(addprefix $(SRC_PATH)/, $(SRC))

OBJS =  $(addprefix $(OBJ_PATH)/, $(SRC:%.cpp=%.o))

SRCS_TEST = $(addprefix $(TEST_PATH)/, $(SRC))

OBJS_TEST =  $(addprefix $(TEST_PATH)/, $(SRC:%.cpp=%.o))

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)
	mkdir -p $(addprefix $(OBJ_PATH)/, $(SUBFILE1_PATH))
	mkdir -p $(addprefix $(OBJ_PATH)/, $(SUBFILE2_PATH))
	mkdir -p $(addprefix $(OBJ_PATH)/, $(SUBFILE3_PATH))
	mkdir -p $(addprefix $(OBJ_PATH)/, $(SUBFILE4_PATH))
	mkdir -p $(addprefix $(OBJ_PATH)/, $(SUBFILE5_PATH))
	mkdir -p $(addprefix $(OBJ_PATH)/, $(SUBFILE6_PATH))

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp | $(OBJ_PATH)
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS) $(SERVER_MAIN) 
	$(CC) $(CXXFLAGS) $(INCLUDE) $(OBJS) $(SERVER_MAIN) -o $(NAME)
#	clear
	$(GREEN) Program asembled $(RESET)

$(TEST_PATH)/%.o: $(TEST_PATH)/%.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(MOCK): $(OBJS_TEST) $(MOCK_MAIN)
	$(CC) $(CXXFLAGS) $(INCLUDE) $(OBJS_TEST) $(MOCK_MAIN) -o mock
	$(GREEN) Mock program created $(RESET)

$(TEST): $(OBJS_TEST) $(TESTER_MAIN)
	$(CC) $(CXXFLAGS) $(INCLUDE) $(OBJS_TEST) $(TESTER_MAIN) -o $(TEST)
#	clear
	$(GREEN) Program asembled $(RESET)
	@echo "⠀⠀⠀	    ⣠⣴⣶⣿⣿⣷⣶⣄⣀⣀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣾⣿⣿⡿⢿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⡟⠁⣰⣿⣿⣿⡿⠿⠻⠿⣿⣿⣿⣿⣧⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⠏⠀⣴⣿⣿⣿⠉⠀⠀⠀⠀⠀⠈⢻⣿⣿⣇⠀⠀⠀\n\
⠀⠀⠀⠀⢀⣠⣼⣿⣿⡏⠀⢠⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⡀⠀⠀\n\
⠀⠀⠀⣰⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⡇⠀⠀\n\
⠀⠀⢰⣿⣿⡿⣿⣿⣿⡇⠀⠘⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⢀⣸⣿⣿⣿⠁⠀⠀\n\
⠀⠀⣿⣿⣿⠁⣿⣿⣿⡇⠀⠀⠻⣿⣿⣿⣷⣶⣶⣶⣶⣶⣿⣿⣿⣿⠃⠀⠀⠀\n\
⠀⢰⣿⣿⡇⠀⣿⣿⣿⠀⠀⠀⠀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀\n\
⠀⢸⣿⣿⡇⠀⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠉⠛⠛⠛⠉⢉⣿⣿⠀⠀⠀⠀⠀⠀\n\
⠀⢸⣿⣿⣇⠀⣿⣿⣿⠀⠀⠀⠀⠀⢀⣤⣤⣤⡀⠀⠀⢸⣿⣿⣿⣷⣦⠀⠀⠀\n\
⠀⠀⢻⣿⣿⣶⣿⣿⣿⠀⠀⠀⠀⠀⠈⠻⣿⣿⣿⣦⡀⠀⠉⠉⠻⣿⣿⡇⠀⠀\n\
⠀⠀⠀⠛⠿⣿⣿⣿⣿⣷⣤⡀⠀⠀⠀⠀⠈⠹⣿⣿⣇⣀⠀⣠⣾⣿⣿⡇⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣦⣤⣤⣤⣤⣾⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⢿⣿⣿⣿⣿⣿⣿⠿⠋⠉⠛⠋⠉⠉⠁⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠁\n"

$(MAKE): make

clean:
	$(PURPLE) CLEANING OBJECTS $(RESET)
	rm -rf $(OBJ_PATH)
	rm -f $(TEST_PATH)/main.o
	rm -f $(TEST_PATH)/mock_main.o
	rm -f $(TEST_PATH)/*/*.o

fclean: clean
	$(PURPLE) CLEANING OBJECTS AND EXEC $(RESET)
	rm -rf $(NAME)
	rm -rf $(TEST)
	rm -rf $(MOCK)

re: fclean all


PHONY.: all clean fclean re
