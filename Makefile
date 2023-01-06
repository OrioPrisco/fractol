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
					deal_key.c\
					color_chunk.c\
					mandelbrot.c\
					debug.c\
					camera.c\
					winding.c\
					image.c\
					deepen.c\


OBJ_FOLDER		=	srcs/

CFLAGS			=	-Wall -Wextra -Werror

SRC_FOLDER		=	srcs/

HEADERS_FOLDER	=	includes/\
					minilibx-linux/\
					libft/includes/

OBJS = $(patsubst %.c,$(OBJ_FOLDER)%.o,$(SRC))

DEPENDS := $(patsubst %.c,$(OBJ_FOLDER)%.d,$(SRC))

all: $(NAME)

bonus: all

$(NAME): $(OBJS) $(LIBS)
	cc $(CFLAGS) $(OBJS) -Llibft -lft -Lminilibx-linux -lmlx -lXext -lX11 -lm -lbsd -o $(NAME)

-include $(DEPENDS)

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.c Makefile
	$(CC) -c $(CFLAGS) $(addprefix -I,$(HEADERS_FOLDER)) -MMD -MP $< -o $@

clean:
	rm -f $(OBJS) $(LIBS)

fclean: clean
	rm -f $(NAME)

re: fclean all
	
.PHONY: all clean fclean re bonus
.SUFFIXES:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
