/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_chunk.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 11:59:24 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/28 12:52:04 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

const int		g_palette[] = {
	0x0000ff,
	0x00ff00,
	0x000f60,
	0xff0000,
	0x0f6000
};
const int		g_palette2[] = {
	0xb40982,
	0x4e54cb,
	0x4e94cb,
	0x4edfcb,
	0x4edf00,
	0xb1df00,
	0xffdf00,
	0xff9a00,
	0xff6b00,
	0xff5700
};

static int	get_color(int i, int iter)
{
	if (i == iter)
		return (0);
	return (g_palette2[i % 9]);
}

void	color_uniform_chunk(t_img *img, t_chunk *chunk, int iter)
{
	int	x;
	int	y;
	int	color;

	y = chunk->top_left[1];
	color = get_color(chunk->borders[0][0].iter, iter);
	while (y < chunk->top_left[1] + chunk->dimensions[1])
	{
		x = chunk->top_left[0];
		while (x < chunk->top_left[0] + chunk->dimensions[0])
		{
			my_mlx_pixel_put(img, x, y, color);
			x++;
		}
		y++;
	}
}

void	color_small_chunk(t_img *img, t_chunk *chunk, int iter)
{
	int	x;
	int	y;
	int	color;

	y = chunk->top_left[1];
	color = get_color(chunk->borders[0][0].iter, iter);
	while (y < chunk->top_left[1] + chunk->dimensions[1])
	{
		x = chunk->top_left[0];
		while (x < chunk->top_left[0] + chunk->dimensions[0])
		{
			if (y == chunk->top_left[1])
				color = get_color(chunk->borders[U]
					[x - chunk->top_left[0]].iter, iter);
			else if (x == chunk->top_left[0])
				color = get_color(chunk->borders[L]
					[y - chunk->top_left[1]].iter, iter);
			my_mlx_pixel_put(img, x, y, color);
			x++;
		}
		y++;
	}
}
