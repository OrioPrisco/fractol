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
#include "fractol.h"
#include <X11/keysym.h>

int	quit_prg(t_env *env)
{
	mlx_destroy_image(env->mlx, env->frame->img);
	mlx_destroy_window(env->mlx, env->win);
	mlx_destroy_display(env->mlx);
	free(env->mlx);
	exit(0);
}

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	unsigned char	*dest;

	dest = img->data + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dest = color;
}

const int		g_palette[] = {
	0x0000ff,
	0x00ff00,
	0x000f60,
	0xff0000,
	0x0f6000
};
const int		g_palette2[] = {
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

//TODO : if at any point in the iterations z is in this circle
//then it is definitely in the set
const t_complex	g_offset = {0.25, 0}; // dist = 0.5

void	draw(t_env *env)
{
	int			x;
	int			y;
	int			iter;
	t_complex	z;
	t_complex	c;
	t_complex	top_left;

	top_left.real = (env->camera_center.real - ((env->frame->width / env->scale) / 2));
	top_left.imag = (env->camera_center.imag - ((env->frame->height / env->scale) / 2));
	y = 0;
	while (y < env->frame->height)
	{
		x = 0;
		while (x < env->frame->width)
		{
			c.real = top_left.real + x / env->scale;
			c.imag = top_left.imag + y / env->scale;
			z = c;
			iter = 0;
			my_mlx_pixel_put(env->frame, x, y, 0x0);
			while (iter++ < env->iter)
			{
				z = add_complex(square_complex(z), c);
				if (dist_origin_squared(z) > 4)
				{
					my_mlx_pixel_put(env->frame, x, y, g_palette2[iter % 9]);
					break ;
				}
			}
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(env->mlx, env->win, env->frame->img, 0, 0);
}

int	deal_key(int key, t_env *env)
{
	write(1, &key, 1);
	if (key == XK_P)
		env->iter++;
	if (key == XK_M)
		env->iter--;
	if (key == XK_8)
		env->scale *= 1.1;
	if (key == XK_slash)
		env->scale /= 1.1;
	if (key == XK_W || key == XK_Up)
		env->camera_center.imag -= env->frame->height / (env->scale * 20);
	if (key == XK_S || key == XK_Down)
		env->camera_center.imag += env->frame->height / (env->scale * 20);
	if (key == XK_A || key == XK_Left)
		env->camera_center.real -= env->frame->width / (env->scale * 20);
	if (key == XK_D || key == XK_Right)
		env->camera_center.real += env->frame->width / (env->scale * 20);
	if (key == XK_Escape)
		quit_prg(env);
	printf("\ncenter : %f %f\nscale : %f\n", env->camera_center.real, env->camera_center.imag, env->scale);
	draw(env);
	return (0);
}

int	my_expose(t_env *env)
{
	mlx_put_image_to_window(env->mlx, env->win, env->frame->img, 0, 0);
	return (0);
}

int	main(void)
{
	t_env	env;
	t_img	img;

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
	img.data = (unsigned char *)mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	img.height = HEIGHT;
	img.width = WIDTH;
	env.camera_center.real = 0;
	env.camera_center.imag = 0;
	printf("image %p\n", img.img);
	mlx_key_hook(env.win, &deal_key, &env);
	mlx_expose_hook(env.win, my_expose, &env);
	mlx_hook(env.win, DestroyNotify, StructureNotifyMask, &quit_prg, &env);
	draw(&env);
	mlx_loop(env.mlx);
}
