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
#include "color_chunk.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void	draw_3b1b_dbg(t_env *env);
int		deepen_chunk(t_camera *camera, t_chunk *chunk,
			t_f_iterator *f, void *data);

int	my_loop_hook(t_env *env)
{
	switch_frame(env);
	mlx_clear_window(env->mlx, env->win);
	if (env->camera.chunk && env->camera.iter < 3000)
	{
		if (deepen_chunk(&env->camera, env->camera.chunk,
				mandelbrot_iterate, 0))
		{
			free_chunk(env->camera.chunk, 1);
			env->camera.chunk = 0;
		}
	}
	mlx_put_image_to_window(
		env->mlx, env->win, env->camera.work_buffer.img, 0, 0);
	return (0);
}

void	draw(t_env *env)
{
	switch_frame(env);
	mlx_clear_window(env->mlx, env->win);
	free_chunk(env->camera.chunk, 1);
	if (env->camera.debug & DBG_WINDING)
		draw_3b1b_dbg(env);
	else
		env->camera.chunk = boundary_trace_fractal
			(&env->camera, mandelbrot_iterate, 0);
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
	if (env->camera.chunk)
		free_chunk(env->camera.chunk, 1);
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
	env->camera.iter = 10;
	draw(env);
	return (0);
}

int	my_expose(t_env *env)
{
	mlx_put_image_to_window(env->mlx, env->win, env->frame->img, 0, 0);
	return (0);
}
