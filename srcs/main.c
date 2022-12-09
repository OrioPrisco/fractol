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

int	deal_key(int key, t_env *env)
{
	write(1, &key, 1);
	if (key == XK_p)
		env->iter++;
	if (key == XK_m)
		env->iter--;
	if (key == XK_8)
		env->scale *= 1.1;
	if (key == XK_slash)
		env->scale /= 1.1;
	if (key == XK_w || key == XK_Up)
		env->camera_center.imag -= env->frame->height / (env->scale * 20);
	if (key == XK_s || key == XK_Down)
		env->camera_center.imag += env->frame->height / (env->scale * 20);
	if (key == XK_a || key == XK_Left)
		env->camera_center.real -= env->frame->width / (env->scale * 20);
	if (key == XK_d || key == XK_Right)
		env->camera_center.real += env->frame->width / (env->scale * 20);
	if (key == XK_Escape)
		quit_prg(env);
	printf("\ncenter: %f %f\nscale: %f\n iter: %d\n", env->camera_center.real,
		env->camera_center.imag, env->scale, env->iter);
	draw(env);
	return (0);
}

int	my_mouse_hook(int button, int x, int y, t_env *env)
{
	t_complex	top_left;

	printf("mouse event : %d \n", button);
	if (button == 4)
		env->scale *= 1.1;
	if (button == 5)
		env->scale /= 1.1;
	if (button == 1)
	{
		top_left.real = (env->camera_center.real
				- ((env->frame->width / env->scale) / 2));
		top_left.imag = (env->camera_center.imag
				- ((env->frame->height / env->scale) / 2));
		top_left.real = top_left.real + x / env->scale;
		top_left.imag = top_left.imag + y / env->scale;
		env->camera_center = top_left;
	}
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
	mlx_mouse_hook(env.win, my_mouse_hook, &env);
	draw(&env);
	mlx_loop(env.mlx);
}
