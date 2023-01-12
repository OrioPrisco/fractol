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

#include "camera.h"
#include "libft.h"
#include "color_chunk.h"
#include <stdlib.h>
#include "mlx.h"
#include "winding.h"
#include "fractals.h"

int	boundary_trace_fractal_r(t_camera *camera, t_chunk *chunk,
		t_f_iterator *f, t_param *data);

//TODO : if at any point in the iterations z is in one of these circles
//       then it is definitely in the set
//       or maybe not
//       maybe only check this after at least 10/20 iterations,
//       then i think it's safe
//       make my own tools to check that
const t_complex	g_offset = {0.25, 0}; // dist = 0.5 // 
const t_complex	g_offset2 = {0, 0}; // dist = 0.25 //super cheap to check

void	free_chunk(t_chunk	*chunk, int is_top_chunk)
{
	t_direction	dir;

	if (!chunk)
		return ;
	dir = -1;
	if (is_top_chunk)
		while (++dir < 4)
			free(chunk->borders[dir]);
	if (chunk->childs)
	{
		free_chunk(&chunk->childs->c1, 0);
		free_chunk(&chunk->childs->c2, 0);
		free(chunk->childs);
	}
	if (is_top_chunk)
		free(chunk);
}

//TODO : determine how many iterations are stricly required for crtain scale
//       and how many are recommended. Then increase iterations 
//       in the background when you have nothing to do
// depending on where we are, values are more or less stable
// near the tangent part of the cardiod and 1st continent, values are very
// stable, and might require infinitely many iterations
// seems like iterative deepening is the way
static void	iterate_chunk_borders(t_camera *camera, t_chunk *chunk,
	t_f_iterator *f, t_param *data)
{
	t_direction	dir;
	size_t		i;

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
}

static int	subdivide_chunk(t_camera *camera, t_chunk *c,
	t_f_iterator *f, t_param *data)
{
	int				h;
	t_chunk_childs	*childs;

	h = !(!(c->filled & 1 << UP) || !(c->filled & 1 << DOWN));
	childs = malloc(sizeof(*childs)
			+ (sizeof(**c->borders) * c->dimensions[!h]));
	if (!childs)
		return (1);
	childs->c1 = *c;
	childs->c2 = *c;
	childs->c1.dimensions[h] -= c->dimensions[h] / 2;
	childs->c2.dimensions[h] -= childs->c1.dimensions[h];
	c->childs = childs;
	childs->c1.filled = ~(1 << (LEFT - h) % 4);
	childs->c2.filled = ~(1 << (RIGHT - h) % 4);
	childs->c1.top_left[h] += c->dimensions[h] / 2;
	childs->c1.borders[(U + h) % 4] += childs->c2.dimensions[h];
	childs->c1.borders[(D + h) % 4] += childs->c2.dimensions[h];
	childs->c1.borders[(L - h) % 4] = (t_iter_result *)
		(((char *)childs) + sizeof(*childs));
	childs->c2.borders[(R - h) % 4] = childs->c1.borders[(LEFT - h) % 4];
	iterate_chunk_borders(camera, &childs->c1, f, data);
	boundary_trace_fractal_r(camera, &childs->c1, f, data);
	boundary_trace_fractal_r(camera, &childs->c2, f, data);
	return (0);
}

int	boundary_trace_fractal_r(t_camera *camera, t_chunk *chunk,
	t_f_iterator *f, t_param *data)
{
	t_direction	dir;
	size_t		i;

	dir = -1;
	if (chunk->dimensions[0] < 2 || chunk->dimensions[1] < 2)
		return (color_small_chunk
			(&camera->work_buffer, chunk, camera), 0);
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
		(&camera->work_buffer, chunk, camera), 0);
}

t_chunk	*boundary_trace_fractal(t_camera *camera,
	t_f_iterator *f, t_param *data)
{
	t_chunk	*chnk;

	chnk = ft_calloc(sizeof(*chnk), 1);
	if (!chnk)
		return (0);
	chnk->dimensions[0] = camera->work_buffer.width;
	chnk->dimensions[1] = camera->work_buffer.height;
	chnk->borders[U] = malloc(sizeof(**chnk->borders) * chnk->dimensions[0]);
	chnk->borders[L] = malloc(sizeof(**chnk->borders) * chnk->dimensions[1]);
	chnk->borders[D] = malloc(sizeof(**chnk->borders) * chnk->dimensions[0]);
	chnk->borders[R] = malloc(sizeof(**chnk->borders) * chnk->dimensions[1]);
	if (!chnk->borders[U] || !chnk->borders[L] || !chnk->borders[D]
		|| !chnk->borders[R])
		return (free_chunk(chnk, 1), NULL);
	iterate_chunk_borders(camera, chnk, f, data);
	if (boundary_trace_fractal_r(camera, chnk, f, data))
		return (free_chunk(chnk, 1), NULL);
	return (chnk);
}
