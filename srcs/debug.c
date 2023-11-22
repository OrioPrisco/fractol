/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 14:04:12 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/28 16:31:30 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "fractals.h"
#include "color_chunk.h"
#include <math.h>

const t_complex_part	g_tau = 2 * M_PI;

static t_complex_part	get_angle(t_complex num)
{
	if (num.imag < 0)
		return (acos(num.real / sqrt(dist_origin_squared(num))));
	return (2 * M_PI - acos(num.real / sqrt(dist_origin_squared(num))));
}

static t_complex	mandelbrot_i(t_complex z, t_complex c, size_t iterations)
{
	size_t		iter;
	t_complex	num;

	num = z;
	iter = iterations;
	while (iter--)
	{
		num = add_complex(square_complex(num), c);
	}
	return (num);
}

static int	angle_to_color(t_complex_part angle)
{
	int	red;
	int	green;
	int	blue;

	red = 0;
	green = 0;
	blue = 0xff * (1.0 - ((angle - g_tau * (5.0 / 6.0)) / (g_tau / 6.0)));
	if (angle < g_tau / 6.0 || angle > g_tau * (5.0 / 6.0))
		red = 0xff;
	else if (angle < g_tau / 3.0)
		red = 0xff * (1.0 - ((angle - g_tau / 6.0) / (g_tau / 6.0)));
	else if (angle > g_tau * (2.0 / 3.0))
		red = 0xff * ((angle - g_tau * (2.0 / 3.0)) / (g_tau / 6.0));
	if (angle < g_tau / 6.0)
		green = 0xff * (angle / (g_tau / 6.0));
	else if (angle < g_tau / 2.0)
		green = 0xff;
	else if (angle < g_tau * (2.0 / 3.0))
		green = 0xff * (1.0 - ((angle - g_tau / 2.0) / (g_tau / 6.0)));
	blue = blue * (angle >= g_tau / 3.0);
	if (blue != 0 && angle < g_tau / 2.0)
		blue = 0xff * ((angle - g_tau / 3.0) / (g_tau / 6.0));
	else if (blue != 0 && angle < g_tau * (5.0 / 6.0))
		blue = 0xff;
	return ((red << 16) + (green << 8) + blue);
}

void	draw_3b1b_dbg(t_env *env)
{
	int			x;
	int			y;
	size_t		iter;
	t_complex	c;
	t_complex	z;

	y = -1;
	while (++y < env->camera.work_buffer.height)
	{
		x = -1;
		while (++x < env->camera.work_buffer.width)
		{
			c.real = env->camera.top_left.real + x * env->camera.step.real;
			c.imag = env->camera.top_left.imag + y * env->camera.step.imag;
			z = complex(0, 0);
			iter = mandelbrot_iterate(&z, c, env->camera.iter + 1, 0);
			if (!(env->camera.debug & DBG_WINDING_STEP))
				z = mandelbrot_i(z, c, env->camera.iter + 1 - iter);
			if (iter == (size_t)env->camera.iter + 1)
				my_mlx_pixel_put(&env->camera.work_buffer, x, y, 0x0);
			else
				my_mlx_pixel_put(&env->camera.work_buffer, x, y,
					angle_to_color(get_angle(z)));
		}
	}
}
