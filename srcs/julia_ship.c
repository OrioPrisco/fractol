/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia_ship.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 11:54:05 by OrioPrisc         #+#    #+#             */
/*   Updated: 2023/01/11 11:40:47 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"

size_t	julia_ship_iterate(t_complex *z, t_complex c, size_t iterations,
			void *data)
{
	t_complex	temp;
	size_t		iter;

	temp = c;
	c = *z;
	*z = temp;
	iter = burning_ship_iterate(&c, *(t_complex *)data, iterations, 0);
	temp = c;
	c = *z;
	*z = temp;
	return (iter);
}
