/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:01:14 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/28 12:57:59 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_hooks.h"
#include "fractol.h"
#include <X11/X.h>
#include <X11/keysym.h>
#include "mlx.h"
#include "fractals.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void	draw_3b1b_dbg(t_env *env);

void	draw(t_env *env)
{
	if (env->debug)
		draw_3b1b_dbg(env);
	else
		boundary_trace_fractal(env, mandelbrot_iterate);
	mlx_put_image_to_window(env->mlx, env->win, env->frame->img, 0, 0);
}

int	quit_prg(t_env *env)
{
	if (!env->mlx)
		exit(0);
	if (env->frame->img)
		mlx_destroy_image(env->mlx, env->frame->img);
	if (env->win)
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
	if (key == XK_F1)
		env->debug = !env->debug;
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
