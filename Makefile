# COLORS #
#
GREEN 	= @echo "\033[0;32m"
BLUE 	= @echo "\033[0;34m" 
PURPLE 	= @echo "\033[0;35m"
CYAN 	= @echo "\033[0;36m"
RESET 	= "\033[1;0m"

# VARIABLES #
#
NAME 		= irc
CC 			= clang++
INCLUDE 	= -std=c++98
CXXFLAGS 	= -Wall -Werror -Wextra

# PATHs #
#
SRC_PATH    	= srcs
SUBFILE1_PATH   = parser
OBJ_PATH    	= objects

# SOURCES #

SUBFILE1_SRC = test.cpp

SRC =  	main.cpp \
		$(addprefix $(SUBFILE1_PATH)/, $(SUBFILE1_SRC))



# RULES #
all: $(NAME)

SRCS = $(addprefix $(SRC_PATH)/, $(SRC))

OBJS =  $(addprefix $(OBJ_PATH)/, $(SRC:%.cpp=%.o))



$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)
	mkdir -p $(addprefix $(OBJ_PATH)/, $(SUBFILE1_PATH))


$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp | $(OBJ_PATH)
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
            
$(NAME): $(OBJS)
	$(CC) $(CXXFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)
	clear
	$(GREEN) Program asembled $(RESET)

##RULES
$(MAKE): make

clean:
	$(PURPLE) CLEANING OBJECTS $(RESET)
	rm -rf $(OBJ_PATH)
	
fclean: clean
	$(PURPLE) CLEANING DIRECTORY AND EXEC $(RESET)
	rm -rf $(NAME)
	rm -rf $(OBJ_PATH)

re: fclean all

PHONY.: all clean fclean re
