/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 11:24:44 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/09 12:32:21 by OrioPrisc        ###   ########.fr       */
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

static int	subdivide_chunk(t_camera *camera, t_chunk *chunk,
	t_fractal_iterator *f, void *data)
{
	t_chunk			c1;
	t_chunk			c2;
	int				h_split;
	t_iter_result	shared_border[WIDTH + HEIGHT];

	c1 = *chunk;
	c2 = *chunk;
	h_split = !(!(chunk->filled & 1 << UP) || !(chunk->filled & 1 << DOWN));
	c1.filled = ~(1 << (LEFT - h_split) % 4);
	c2.filled = ~(1 << (RIGHT - h_split) % 4);
	c1.top_left[h_split] += chunk->dimensions[h_split] / 2;
	c1.dimensions[h_split] -= chunk->dimensions[h_split] / 2;
	c2.dimensions[h_split] -= c1.dimensions[h_split];
	c1.borders[(UP + h_split) % 4] += c2.dimensions[h_split];
	c1.borders[(DOWN + h_split) % 4] += c2.dimensions[h_split];
	c1.borders[(LEFT - h_split) % 4] = shared_border;
	c2.borders[(RIGHT - h_split) % 4] = shared_border;
	iterate_chunk_borders(camera, &c1, f, data);
	boundary_trace_fractal_r(camera, &c1, f, data);
	boundary_trace_fractal_r(camera, &c2, f, data);
	return (0);
}

static int	boundary_trace_fractal_r(t_camera *camera, t_chunk *chunk,
	t_fractal_iterator *f, void *data)
{
	int	dir;
	int	i;

	dir = -1;
	if (chunk->dimensions[0] < 2 || chunk->dimensions[1] < 2)
		return (color_small_chunk
			(&camera->work_buffer, chunk, camera->iter, camera->debug), 0);
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
	return (color_uniform_chunk
		(&camera->work_buffer, chunk, camera->iter, camera->debug), 0);
}

int	boundary_trace_fractal(t_camera *camera,
	t_fractal_iterator *f, void *data)
{
	t_chunk			chnk;
	t_iter_result	borders[4][WIDTH + HEIGHT];

	chnk.top_left[0] = 0;
	chnk.top_left[1] = 0;
	chnk.dimensions[0] = camera->work_buffer.width;
	chnk.dimensions[1] = camera->work_buffer.height;
	chnk.borders[UP] = borders[UP];
	chnk.borders[LEFT] = borders[LEFT];
	chnk.borders[DOWN] = borders[DOWN];
	chnk.borders[RIGHT] = borders[RIGHT];
	chnk.filled = 0;
	return (boundary_trace_fractal_r(camera, &chnk, f, data));
}
