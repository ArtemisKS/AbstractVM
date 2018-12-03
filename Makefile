# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akupriia <akupriia@student.unit.ua>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/27 19:23:53 by akupriia          #+#    #+#              #
#    Updated: 2018/12/03 03:18:37 by akupriia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = avm

INCDIR = ./includes
SRCDIR = ./sources

FILENAMES =	main \
			OperandFactory \
			AVM

CFILES = $(patsubst %, $(SRCDIR)/%.cpp, $(FILENAMES))
OFILES = $(patsubst %, $(ODIR)/%.o, $(FILENAMES))

FLAGS =  -std=c++11 -Wall -Wextra -Werror 
ODIR = ./obj
CC = clang++

BLACK = '\033[0;30m'
RED = '\033[0;31m'
GREEN = '\033[0;32m'
BROWN = '\033[0;33m'
BLUE = '\033[0;34m'
PURPLE = '\033[0;35m'
CYAN = '\033[0;36m'
LGRAY = '\033[0;37m'
DGRAY = '\033[1;30m'
LRED = '\033[1;31m'
LGREEN = '\033[1;32m'
YELLOW = '\033[1;33m'
LBLUE = '\033[1;34m'
PINK = '\033[1;35m'
LCYAN = '\033[1;36m'
WHITE = '\033[1;37m'
RESET = '\033[0m' # No Color

all: $(NAME)

$(NAME): $(ODIR) $(OFILES)
	@echo ${CYAN}[Compiling $(NAME)]${RESET}
	@$(CC) $(FLAGS) $(OFILES) $(LIBFT) -o $(NAME) 
	@echo ${GREEN}" Done, $(NAME) is ready "${RESET}

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	@$(CC) $(FLAGS) -o $@ -c $< -I$(INCDIR)
	@echo "\033[0;42m  \033[0m\c"

$(ODIR):
	@mkdir -p $(ODIR)

clean:
	@echo ${RED}[Removing *.o files]${RESET}
	@/bin/rm -rf $(ODIR)
	
fclean: clean
	@echo ${RED}[Removing $(NAME) executable]${RESET}
	@/bin/rm -f $(NAME)

re: fclean all
