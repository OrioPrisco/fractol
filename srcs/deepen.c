/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deepen.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 15:59:37 by OrioPrisc         #+#    #+#             */
/*   Updated: 2023/01/04 18:28:17 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "fractals.h"
#include "color_chunk.h"

static void	reiterate_chunk_borders(t_camera *camera, t_chunk *chunk,
	t_f_iterator *f, void *data)
{
	t_direction	dir;
	int			i;

	dir = -1;
	while (++dir < 4)
	{
		if (chunk->filled & (1 << dir))
			continue ;
		i = 0;
		while (i < chunk->dimensions[dir % 2])
		{
			if (chunk->borders[dir][i].iter == camera->iter - 1)
				chunk->borders[dir][i].iter += f
					(&chunk->borders[dir][i].z, chunk->borders[dir][i].c,
						1, data);
			i++;
		}
	}
}

int	boundary_trace_fractal_r(t_camera *camera, t_chunk *chunk,
				t_f_iterator *f, void *data);

static int	deepen_chunk_r(t_camera *camera, t_chunk *chunk,
	t_f_iterator *f, void *data)
{
	reiterate_chunk_borders(camera, chunk, f, data);
	if (chunk->childs)
	{
		if (deepen_chunk_r(camera, &chunk->childs->c1, f, data))
			return (1);
		if (deepen_chunk_r(camera, &chunk->childs->c2, f, data))
			return (1);
		return (0);
	}
	else
	{
		return (boundary_trace_fractal_r(camera, chunk, f, data));
	}
}

//pass by how much to deepen ?
//returns 0 on success
//returns 1 on failure, and destroys the t_chunk*
int	deepen_chunk(t_camera *camera, t_chunk *chunk,
	t_f_iterator *f, void *data)
{
	camera->iter++;
	if (deepen_chunk_r(camera, chunk, f, data))
		return (free_chunk(chunk, 1), 1);
	return (0);
}
