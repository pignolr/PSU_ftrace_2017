##
## EPITECH PROJECT, 2018
## ftrace
## File description:
## Makefile
##

CC		= gcc

CFLAGS		+= -W -Wall -Wextra -Werror

CPPFLAGS	+= -Iinc -D_GNU_SOURCE

LDFLAGS		+= -lelf

LDLIBS		+=

NAME		= ftrace

SRCS		=\
		src/init_trace.c \
		src/ftrace.c \
		src/analyse.c \
		src/print_exit.c \
		src/get_arguments.c \
		src/elf_file.c \
		src/print_syscall.c \
		src/get_path.c \
		src/print_function.c \
		src/main.c

OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
