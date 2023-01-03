/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 11:24:44 by OrioPrisco         #+#    #+#             */
/*   Updated: 2023/01/03 16:32:30 by OrioPrisco       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "libft.h"
#include "color_chunk.h"
#include <stdlib.h>
#include <stdio.h>
#include "mlx.h"
#include "winding.h"
#include "fractals.h"

static int	boundary_trace_fractal_r(t_camera *camera, t_chunk *chunk,
				t_fractal_iterator *f, void *data);

//TODO : if at any point in the iterations z is in one of these circles
//       then it is definitely in the set
//       or maybe not
//       maybe only check this after at least 10/20 iterations,
//       then i think it's safe
//       make my own tools to check that
const t_complex	g_offset = {0.25, 0}; // dist = 0.5 // 
const t_complex	g_offset2 = {0, 0}; // dist = 0.25 //super cheap to check

static void	free_chunk(t_chunk	*chunk)
{
	t_direction	dir;

	if (!chunk)
		return ;
	dir = -1;
	while (++dir < 4)
		if (!(chunk->filled & (1 << dir)))
			free(chunk->borders[dir]);
	if (chunk->childs[0] && chunk->childs[1])
	{
		while (dir < 4)
		{
			if (!(chunk->childs[0]->filled & (1 << dir)))
			{
				free(chunk->childs[0]->borders[dir]);
				chunk->childs[1]->borders[(dir + 2) % 4] = 0;
			}
			dir++;
		}
	}
	free_chunk(chunk->childs[0]);
	free_chunk(chunk->childs[1]);
	free(chunk);
}

//TODO : determine how many iterations are stricly required for crtain scale
//       and how many are recommended. Then increase iterations 
//       in the background when you have nothing to do
// depending on where we are, values are more or less stable
// near the tangent part of the cardiod and 1st continent, values are very
// stable, and might require infinitely many iterations
// seems like iterative deepening is the way
static int	iterate_chunk_borders(t_camera *camera, t_chunk *chunk,
	t_fractal_iterator *f, void *data)
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
			chunk->borders[dir][i].z = add_complex(add_complex(
						camera->top_left, complex
						(((dir == 3) * chunk->dimensions[0] + i * (!(dir % 2)))
							* camera->step.real, ((dir == D) * chunk->dimensions
							[1] + i * (dir % 2)) * camera->step.imag)),
					complex(chunk->top_left[0] * camera->step.real,
						chunk->top_left[1] * camera->step.imag));
			chunk->borders[dir][i].c = chunk->borders[dir][i].z;
			chunk->borders[dir][i].iter = f(&chunk->borders[dir][i].z,
					chunk->borders[dir][i].z, camera->iter, data);
			i++;
		}
	}
	return (0);
}

static int	subdivide_chunk(t_camera *camera, t_chunk *c,
	t_fractal_iterator *f, void *data)
{
	t_chunk			*c1;
	t_chunk			*c2;
	int				h;

	c1 = malloc(sizeof(*c1));
	c2 = malloc(sizeof(*c2));
	if (!c1 || !c2)
		return (free(c1), free(c2), 1);
	*c1 = *c;
	*c2 = *c;
	h = !(!(c->filled & 1 << UP) || !(c->filled & 1 << DOWN));
	c1->filled = ~(1 << (LEFT - h) % 4);
	c2->filled = ~0;
	c1->top_left[h] += c->dimensions[h] / 2;
	c1->dimensions[h] -= c->dimensions[h] / 2;
	c2->dimensions[h] -= c1->dimensions[h];
	c1->borders[(U + h) % 4] += c2->dimensions[h];
	c1->borders[(D + h) % 4] += c2->dimensions[h];
	c1->borders[(L - h) % 4] = malloc(sizeof(**c->borders) * c->dimensions[!h]);
	c2->borders[(R - h) % 4] = c1->borders[(LEFT - h) % 4];
	if (!c1->borders[(LEFT - h) % 4])
		return (free(c1), free(c2), 1);
	boundary_trace_fractal_r(camera, c1, f, data);
	boundary_trace_fractal_r(camera, c2, f, data);
	return (free(c1->borders[(LEFT - h) % 4]), free(c1), free(c2), 0);
}

static int	boundary_trace_fractal_r(t_camera *camera, t_chunk *chunk,
	t_fractal_iterator *f, void *data)
{
	int	dir;
	int	i;

	if (iterate_chunk_borders(camera, chunk, f, data))
		return (1);
	dir = -1;
	if (chunk->dimensions[0] < 2 || chunk->dimensions[1] < 2)
		return (color_small_chunk
			(&camera->work_buffer, chunk, camera->iter), 0);
	while (++dir < 4)
	{
		i = 0;
		while (i < chunk->dimensions[dir % 2])
		{
			if (chunk->borders[0][0].iter != chunk->borders[dir][i].iter)
				return (subdivide_chunk(camera, chunk, f, data));
			i++;
		}
	}
	if (chunk->borders[0][0].iter != camera->iter && contains_zero(chunk))
		return (subdivide_chunk(camera, chunk, f, data));
	return (color_uniform_chunk(&camera->work_buffer, chunk, camera->iter), 0);
}

t_chunk	*boundary_trace_fractal(t_camera *camera,
	t_fractal_iterator *f, void *data)
{
	t_chunk	*chnk;
	int		res;

	chnk = malloc(sizeof(*chnk));
	if (!chnk)
		return (1);
	chnk->childs[0] = 0;
	chnk->childs[1] = 0;
	chnk->top_left[0] = 0;
	chnk->top_left[1] = 0;
	chnk->dimensions[0] = camera->work_buffer.width;
	chnk->dimensions[1] = camera->work_buffer.height;
	chnk->filled = 0;
	chnk->borders[U] = malloc(sizeof(**chnk->borders) * chnk->dimensions[0]);
	chnk->borders[L] = malloc(sizeof(**chnk->borders) * chnk->dimensions[1]);
	chnk->borders[D] = malloc(sizeof(**chnk->borders) * chnk->dimensions[0]);
	chnk->borders[R] = malloc(sizeof(**chnk->borders) * chnk->dimensions[1]);
	if (!chnk->borders[U] || !chnk->borders[L] || !chnk->borders[D]
		|| !chnk->borders[R])
		return (free_chunk(chnk), 0);
	boundary_trace_fractal_r(camera, chnk, f, data);
	return (chnk);
}
