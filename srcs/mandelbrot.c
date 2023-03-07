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
		if (complex_mod2(num) > 4)
			return (*z = num, iterations - iter);
		iter--;
	}
	return (*z = num, iterations);
}
