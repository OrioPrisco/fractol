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
					parse.c\
					complex.c\
					draw.c\
					env.c\
					hooks.c\
					deal_key.c\
					color_chunk.c\
					mandelbrot.c\
					julia.c\
					burning_ship.c\
					debug.c\
					camera.c\
					winding.c\
					image.c\
					deepen.c\


OBJ_FOLDER		=	objs/

CFLAGS			=	-Wall -Wextra -Werror

SRC_FOLDER		=	srcs/

HEADERS_FOLDER	=	includes/\
					minilibx-linux/\
					libft/includes/


OBJS = $(patsubst %.c,$(OBJ_FOLDER)%.o,$(SRC))

DEPENDS := $(patsubst %.c,$(OBJ_FOLDER)%.d,$(SRC))

all: $(NAME)

bonus: all


minilibx-linux/libmlx.a:
	make -C minilibx-linux MAKEFLAGS=

libft/libft.a:
	make -C libft

libft/libftprintf.a:
	make -C libft

$(NAME): $(OBJS) minilibx-linux/libmlx.a libft/libft.a libft/libftprintf.a
	cc $(CFLAGS) $(OBJS) -lftprintf -Llibft -lft -Lminilibx-linux -lmlx -lXext -lX11 -lm -lbsd -o $(NAME)

-include $(DEPENDS)

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.c Makefile
	$(CC) -c $(CFLAGS) $(addprefix -I,$(HEADERS_FOLDER)) -MMD -MP $< -o $@

clean:
	rm -f $(OBJS) $(DEPENDS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean
	make -C minilibx-linux clean

re: fclean all
	
.PHONY: all clean fclean re bonus
.SUFFIXES:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
