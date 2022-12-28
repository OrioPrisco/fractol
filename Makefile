# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/23 15:27:59 by OrioPrisc         #+#    #+#              #
#    Updated: 2022/12/08 16:03:24 by OrioPrisc        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	fractol
CC				=	cc
SRC				=	main.c\
					complex.c\
					draw.c\
					env.c\
					hooks.c\
					color_chunk.c\
					mandelbrot.c\
					debug.c


SRC_FOLDER		=	srcs/

HEADERS_FOLDER	=	includes/\
					minilibx-linux/\
					libft/includes/

OBJS = $(patsubst %.c,$(OBJ_FOLDER)%.o,$(SRC))

OBJ_FOLDER = objs/

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

bonus: all

$(NAME): $(OBJS) $(LIBS)
	cc $(OBJS) -Llibft -lft -Lminilibx-linux -lmlx -lXext -lX11 -lm -lbsd -o $(NAME)

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.c
	$(CC) -c $(CFLAGS) $(addprefix -I,$(HEADERS_FOLDER)) $< -o $@

clean:
	rm -f $(OBJS) $(LIBS)

fclean: clean
	rm -f $(NAME)

re: fclean all
	
.PHONY: all clean fclean re bonus
.SUFFIXES:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
