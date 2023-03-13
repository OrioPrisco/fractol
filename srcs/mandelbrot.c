/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 11:54:05 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/28 12:55:07 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"
#include "params.h"
#include "camera.h"
#include "color.h"
#include <math.h>

size_t	mandelbrot_iterate(t_complex *z, t_complex c, size_t iterations,
			t_param *data)
{
	size_t		iter;
	t_complex	num;
	size_t		heads;

	num = *z;
	iter = iterations;
	while (iter)
	{
		heads = 0;
		while (heads <= data->mandelbrot_heads)
		{
			num = square_complex(num);
			heads++;
		}
		num = add_complex(num, c);
		if (complex_mod2(num) > (RADIUS * RADIUS))
			return (*z = num, iterations - iter);
		iter--;
	}
	return (*z = num, iterations);
}

int	mandelbrot_smooth(size_t iter, t_complex z, t_camera *camera)
{
	double	smooth_iter;

	smooth_iter = iter + 1 - log(log(complex_mod(z)) / log(RADIUS)) / log(2.0f);
	return (smooth_color(smooth_iter, camera));
}
