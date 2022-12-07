/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 16:09:12 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/07 16:16:42 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <unistd.h>
#include <X11/X.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 500
#define HEIGHT 500
#define SCALE 200.0

typedef struct s_img
{
	void	*img;
	char	*data;
	int		line_length;
	int		bits_per_pixel;
	int		endian;
	int		height;
	int		width;
}	t_img;

typedef struct s_env
{
	void	*win;
	void	*mlx;
	t_img	*frame;
	int		iter;
}	t_env;

typedef struct s_complex
{
	double	real;
	double	imag;
}	t_complex;

typedef struct s_coord
{
	double	x;
	double	y;
}	t_coord;

t_complex	square_complex(t_complex num)
{
	t_complex	ret;

	ret.real = (num.real * num.real) - (num.imag * num.imag);
	ret.imag = 2 * (num.real * num.imag);
	return (ret);
}

t_complex	add_complex(t_complex c1, t_complex c2)
{
	t_complex	result;

	result.real = c1.real + c2.real;
	result.imag = c1.imag + c2.imag;
	return (result);
}

double	dist_complex_origin(t_complex num)
{
	return (num.real * num.real + num.imag * num.imag);
}

int	quit_prg(t_env *env)
{
	mlx_destroy_window(env->mlx, env->win);
	mlx_destroy_display(env->mlx);
	exit(0);
}

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	unsigned char	*dest;
	
	dest = img->data + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int*)dest = color;
}

void draw(t_env *env)
{
	int			x;
	int			y;
	int			iter;
	t_complex	z;
	t_complex	c;

	x = 0;
	while (x < env->frame->width)
	{
		y = 0;
		while (y < env->frame->height)
		{
			c.real = (x - env->frame->width / 2) / SCALE;
			c.imag = (y - env->frame->height / 2) / SCALE;
			z = c;
			iter = 0;
			my_mlx_pixel_put(env->frame, x, y, 0x0000ffff);
			while (iter++ < env->iter)
			{
				z = add_complex(square_complex(z), c);
				if (dist_complex_origin(z) > 4)
				{
					my_mlx_pixel_put(env->frame, x, y, 0x00ff0000);
					break;
				}
			}
			y++;
		}
		x++;
	}
	mlx_put_image_to_window(env->mlx, env->win, env->frame->img, 0, 0);
}

int	deal_key(int key, t_env *env)
{
	write(1, &key, 1);
	
	if (key == 'p')
	{
		env->iter++;
		draw(env);
	}
	if (key == 'm')
	{
		env->iter--;
		draw(env);
	}
	if (key == 65307)
		quit_prg(env);
	return (0);
}

int	my_expose(t_env *env)
{
	mlx_put_image_to_window(env->mlx, env->win, env->frame->img, 0, 0);
}

int main()
{
	t_env env;
	t_img img;

	env.mlx = mlx_init();
	env.frame = &img;
	if (!env.mlx)
		return (1);
	env.win = mlx_new_window(env.mlx, WIDTH, HEIGHT, "Foo bar");
	if (!env.win)
		return (2);
	env.iter = 1;
	img.img = mlx_new_image(env.mlx, WIDTH, HEIGHT);
	img.data = mlx_get_data_addr(img.img, &img.bits_per_pixel,
		&img.line_length, &img.endian);
	img.height = HEIGHT;
	img.width = WIDTH;
	printf("image %p\n", img.img);
	mlx_key_hook(env.win, &deal_key, &env);
//	mlx_loop_hook(env.mlx, my_loop, &env);
	mlx_expose_hook(env.win, my_expose, &env);
	mlx_hook(env.win, DestroyNotify, StructureNotifyMask , &quit_prg, &env);
	draw(&env);
	mlx_loop(env.mlx);
}
