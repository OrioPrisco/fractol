/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 11:54:05 by OrioPrisc         #+#    #+#             */
/*   Updated: 2023/01/09 15:17:13 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"
#include "params.h"

size_t	julia_iterate(t_complex *z, t_complex c, size_t iterations,
			t_param *data)
{
	(void)c;
	return (mandelbrot_iterate(z, data->julia_c, iterations, data));
}
