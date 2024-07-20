INCLUDE = ./include/minishell.h

FILEUTILS = ./utils/errors.c

FILEPARCIN = parsing/create_array.c

FILE = main.c $(FILEUTILS) $(FILEPARCIN) 

CFLAGS = -I/usr/local/opt/readline/include

LDFLAGS = -lreadline

FLAG = cc

GFLAG = $(FLAG) -Wall -Wextra -Werror $(CFLAGS)

OBJECTFILES = $(FILE:.c=.o)

PROGRAM = minishell

LIBFT = ./libft/libft.a

%.o: %.c $(INCLUDE)
	$(GFLAG) -o $@ -c $<

$(PROGRAM): $(OBJECTFILES)
	$(FLAG) $(LDFLAGS) $(OBJECTFILES) $(LIBFT) -o $(PROGRAM)


all: $(PROGRAM)

clean:
	rm -f $(OBJECTFILES)

fclean: clean
	rm -f $(PROGRAM)

re: fclean all

.PHONY: all clean fclean re
