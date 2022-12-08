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

typedef struct s_env
{
	void		*win;
	void		*mlx;
	t_img		*frame;
	int			iter;
	double		scale;
	t_complex	camera_center;
}	t_env;

double		dist_origin_squared(t_complex num);
t_complex	add_complex(t_complex c1, t_complex c2);
t_complex	square_complex(t_complex num);
int	quit_prg(t_env *env);
void	my_mlx_pixel_put(t_img *img, int x, int y, int color);
void draw(t_env *env);
int	deal_key(int key, t_env *env);
int	my_expose(t_env *env);

#endif
