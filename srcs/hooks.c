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
#include "mlx.h"
#include "fractals.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void	draw_3b1b_dbg(t_env *env);

void	draw(t_env *env)
{
	switch_frame(env);
	mlx_clear_window(env->mlx, env->win);
	if (env->camera.debug & DBG_WINDING)
		draw_3b1b_dbg(env);
	else
		boundary_trace_fractal(&env->camera, mandelbrot_iterate, 0);
	mlx_put_image_to_window(
		env->mlx, env->win, env->camera.work_buffer.img, 0, 0);
}

int	quit_prg(t_env *env)
{
	if (!env->mlx)
		exit(0);
	if (env->frame1.img)
		mlx_destroy_image(env->mlx, env->frame1.img);
	if (env->frame2.img)
		mlx_destroy_image(env->mlx, env->frame2.img);
	if (env->camera.work_buffer.img)
		mlx_destroy_image(env->mlx, env->camera.work_buffer.img);
	if (env->win)
		mlx_destroy_window(env->mlx, env->win);
	mlx_destroy_display(env->mlx);
	free(env->mlx);
	exit(0);
}

int	my_mouse_hook(int button, int x, int y, t_env *env)
{
	printf("mouse event : %d \n", button);
	if (button == 4)
		zoom_camera(&env->camera, 1.1);
	if (button == 5)
		zoom_camera(&env->camera, 0.9);
	if (button == 1)
	{
		move_camera(&env->camera,
			complex(
				(x - env->frame->width / 2) / (double)env->frame->width,
				(y - env->frame->height / 2) / (double)env->frame->height));
	}
	draw(env);
	return (0);
}

int	my_expose(t_env *env)
{
	mlx_put_image_to_window(env->mlx, env->win, env->frame->img, 0, 0);
	return (0);
}
