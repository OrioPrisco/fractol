/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metajulia.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 11:54:05 by OrioPrisc         #+#    #+#             */
/*   Updated: 2023/04/07 15:04:12 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"
#include "params.h"

size_t	metajulia_iterate(t_complex *z, t_complex c, size_t iterations,
			t_param *data)
{
	t_complex	temp;
	size_t		iter;

	temp = c;
	c = *z;
	*z = temp;
	iter = metamandelbrot_iterate(&c, data->julia_c, iterations, data);
	temp = c;
	c = *z;
	*z = temp;
	return (iter);
}
