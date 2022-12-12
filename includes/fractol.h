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

typedef struct s_complex
{
	double	real;
	double	imag;
}	t_complex;

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
}	t_direction;

typedef struct s_chunk	t_chunk;

//tree structure
//borders can be shared with childrens, but how to determine who owns a border ?
//I suppose the border belongs to the topmost range, and it will be it's job
//to remove children reference to freed borders
//structure can be kept between runs, and ranges whose borders stopped at the
//previous value of iteration can be deepened
typedef struct s_chunk
{
	t_complex		top_left;
	t_chunk			*subdivisions;
	t_iter_result	*borders[4];
	int				bounds[4];
	char			filled;

}	t_chunk;

double		dist_origin_squared(t_complex num);
t_complex	add_complex(t_complex c1, t_complex c2);
t_complex	square_complex(t_complex num);
int			quit_prg(t_env *env);
void		my_mlx_pixel_put(t_img *img, int x, int y, int color);
void		draw(t_env *env);
int			deal_key(int key, t_env *env);
int			my_expose(t_env *env);
size_t		mandelbrot_iterate(t_complex *z, t_complex c, size_t iterations);
t_complex	calculate_top_left(t_env *env);
int			init_env(t_env *env, t_img *img);

#endif
