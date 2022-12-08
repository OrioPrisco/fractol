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

#define WIDTH 800
#define HEIGHT 800

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

typedef struct s_complex
{
	double	real;
	double	imag;
}	t_complex;

typedef struct s_env
{
	void		*win;
	void		*mlx;
	t_img		*frame;
	int			iter;
	double		scale;
	t_complex	camera_center;
}	t_env;

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

const int g_palette[] = {0x0000ff, 0x00ff00, 0x000f60, 0xff0000, 0x0f6000};
const int g_palette2[] = {
						0xb40982,
						0x4e54cb,
						0x4e94cb,
						0x4edfcb,
						0x4edf00,
						0xb1df00,
						0xffdf00,
						0xff9a00,
						0xff6b00,
						0xff5700
					};

int	min(int a, int b)
{
	return (a<b)?a:b;
}

int	max(int a, int b)
{
	return (a>b)?a:b;
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
			c.real = (env->camera_center.real - ((env->frame->width / env->scale) / 2)) + x / env->scale;
			c.imag = (env->camera_center.imag - ((env->frame->height / env->scale) / 2)) + y / env->scale;
			z = c;
			iter = 0;
			my_mlx_pixel_put(env->frame, x, y, 0x0);
			while (iter++ < env->iter)
			{
				z = add_complex(square_complex(z), c);
				if (dist_complex_origin(z) > 4)
				{
					int color = max(0, min(9, env->iter - iter));
					my_mlx_pixel_put(env->frame, x, y, g_palette2[color]);
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
		env->iter++;
	if (key == 'm')
		env->iter--;
	if (key == '8')
		env->scale *= 1.1;
	if (key == '/')
		env->scale /= 1.1;
	if (key == 'w')
		env->camera_center.imag -= env->frame->height / (env->scale * 20 );
	if (key == 's')
		env->camera_center.imag += env->frame->height / (env->scale * 20);
	if (key == 'a')
		env->camera_center.real -= env->frame->width /  (env->scale * 20);
	if (key == 'd')
		env->camera_center.real += env->frame->width /  (env->scale * 20);
	if (key == 65307)
		quit_prg(env);
	printf("\ncenter : %f %f\nscale : %f\n", env->camera_center.real, env->camera_center.imag, env->scale);
	draw(env);
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
	env.scale = 200;
	img.img = mlx_new_image(env.mlx, WIDTH, HEIGHT);
	img.data = mlx_get_data_addr(img.img, &img.bits_per_pixel,
		&img.line_length, &img.endian);
	img.height = HEIGHT;
	img.width = WIDTH;
	env.camera_center.real = 0;//env.frame->width / 2;
	env.camera_center.imag = 0;//env.frame->height / 2;
	printf("image %p\n", img.img);
	mlx_key_hook(env.win, &deal_key, &env);
//	mlx_loop_hook(env.mlx, my_loop, &env);
	mlx_expose_hook(env.win, my_expose, &env);
	mlx_hook(env.win, DestroyNotify, StructureNotifyMask , &quit_prg, &env);
	draw(&env);
	mlx_loop(env.mlx);
}
