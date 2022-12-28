/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 15:33:21 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/08 15:58:10 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# define WIDTH 800
# define HEIGHT 800
# include <stddef.h>
# include "complex.h"

typedef struct s_img
{
	void			*img;
	unsigned char	*data;
	int				line_length;
	int				bits_per_pixel;
	int				endian;
	int				height;
	int				width;
}	t_img;

typedef struct s_iter_result
{
	t_complex	z;
	t_complex	c;
	int			iter;
}	t_iter_result;

typedef struct s_env
{
	void			*win;
	void			*mlx;
	t_img			*frame;
	int				iter;
	double			scale;
	t_complex		camera_center;
}	t_env;

typedef enum e_direction {
	UP = 0,
	LEFT = 1,
	DOWN = 2,
	RIGHT = 3,
	U = UP,
	L = LEFT,
	D = DOWN,
	R = RIGHT,
}	t_direction;

typedef struct s_chunk	t_chunk;

typedef struct s_chunk
{
	t_iter_result	*borders[4];
	int				top_left[2];
	int				dimensions[2];
	char			filled;

}	t_chunk;

int			quit_prg(t_env *env);
void		my_mlx_pixel_put(t_img *img, int x, int y, int color);
void		draw(t_env *env);
int			deal_key(int key, t_env *env);
int			my_expose(t_env *env);
int			init_env(t_env *env, t_img *img);

#endif
