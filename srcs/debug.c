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

const double	g_tau = 2 * M_PI;

static double	get_angle(t_complex num)
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

static int	angle_to_color(double angle)
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
	t_complex	z;
	t_complex	top_left;

	top_left = calculate_top_left(env);
	y = -1;
	while (++y < env->frame->height)
	{
		x = -1;
		while (++x < env->frame->width)
		{
			z.real = top_left.real + x / env->scale;
			z.imag = top_left.imag + y / env->scale;
			iter = mandelbrot_iterate(&z, complex(0.45, 0.1428), env->iter);
			if (iter == (size_t)env->iter)
				my_mlx_pixel_put(env->frame, x, y, 0x0);
			else
				my_mlx_pixel_put(env->frame, x, y,
					angle_to_color(get_angle(mandelbrot_i(z,
								complex(0.45, 0.1428), env->iter - iter))));
		}
	}
}
