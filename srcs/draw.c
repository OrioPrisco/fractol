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
#include "mlx.h"
#include <stdlib.h>

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	unsigned char	*dest;

	dest = img->data + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dest = color;
}

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

//TODO : if at any point in the iterations z is in one of these circles
//       then it is definitely in the set
//       or maybe not
//       maybe only check this after at least 10/20 iterations,
//       then i think it's safe
//       make my own tools to check that
const t_complex	g_offset = {0.25, 0}; // dist = 0.5 // 
const t_complex	g_offset2 = {0, 0}; // dist = 0.25 //super cheap to check

//TODO : keep an array of WIDTH*HEIGHT t_complexes,
//       to make iterative deepening very cheap
//TODO : determine how many iterations are stricly required for crtain scale
//       and how many are recommended. Then increase iterations 
//       in the background when you have nothing to do
//TODO : calculate the values of countours of rectangles, and if everything is
//		 the same value, flood fill it
//		 otherwise divide into two rectangles and recurse

t_complex	calculate_top_left(t_env *env)
{
	t_complex	top_left;

	top_left.real = (env->camera_center.real
			- ((env->frame->width / env->scale) / 2));
	top_left.imag = (env->camera_center.imag
			- ((env->frame->height / env->scale) / 2));
	return (top_left);
}

void	draw(t_env *env)
{
	int			x;
	int			y;
	int			iter;
	t_complex	c;
	t_complex	top_left;

	top_left = calculate_top_left(env);
	y = 0;
	while (y < env->frame->height)
	{
		x = 0;
		while (x < env->frame->width)
		{
			c.real = top_left.real + x / env->scale;
			c.imag = top_left.imag + y / env->scale;
			iter = mandelbrot_iterate(&c, c, env->iter);
			if (iter == env->iter)
				my_mlx_pixel_put(env->frame, x, y, 0x0);
			else
				my_mlx_pixel_put(env->frame, x, y, g_palette2[iter % 9]);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(env->mlx, env->win, env->frame->img, 0, 0);
}

//either assume that a value inside of env->iter_result set to 0 + 0i
//is unitialized, or somehow pass info to chlidren about which borders were
//calculated
// returns 1 in case of error
void	free_lines(t_iter_result *borders[4], char filled[4])
{
	int	dir;

	dir = 0;
	while (dir < 4)
	{
		if (filled[dir++])
		{
			free(borders[dir]);
		}
	}
}

//if x or y is on the border, use the border value
//this way the same function works for the case where the chunk is very squished
//and the general case
void	color_chunk(t_img *img, t_chunk *chunk)
{
	int	x;
	int	y;
	int	color;

	x = chunk->bounds[LEFT];
	while (x <= chunk->bounds[RIGHT])
	{
		y = chunk->bounds[UP];
		color = g_palette2[chunk->borders[UP][0].iter % 9];
		while (y <= chunk->bounds[LEFT])
		{
			if (x == chunk->bounds[LEFT])
				color = g_palette2[chunk->borders[LEFT]
				[(y - chunk->bounds[UP])].iter % 9];
			if (x == chunk->bounds[RIGHT])
				color = g_palette2[chunk->borders[RIGHT]
				[(y - chunk->bounds[UP])].iter % 9];
			my_mlx_pixel_put(img, x, y, color);
			y++;
		}
	x++;
	}
}

//returns 0 in case of success, or something else if an error occured
//i is ps on screen
//i - off is pos in array
int	iterate_chunk_borders(t_env *env, t_chunk *chunk,
	size_t (*f)(t_complex *, t_complex, size_t))
{
	int	dir;
	int	end;
	int	i;
	int	off;

	dir = -1;
	while (++dir < 4)
	{
		if (chunk->filled & (1 << dir))
			continue ;
		i = chunk->bounds[(dir + 1) % 2];
		off = i;
		end = chunk->bounds[2 + ((dir + 1) % 2)];
		chunk->borders[dir] = ft_calloc(end - i, sizeof(**(chunk->borders)));
		if (!chunk)
			return (free_lines(chunk->borders, &chunk->filled), 1);
		while (i < end)
		{
			chunk->borders[i - off]->z.real = i * (dir % 2) / env->scale;
			chunk->borders[i - off]->z.imag = i * ((dir + 1) % 2) / env->scale;
			chunk->borders[i - off]->z = add_complex(chunk->borders[i - off]->z,
					chunk->top_left);
			chunk->borders[i - off]-> c = chunk->borders[i - off]->c;
			chunk->borders[i - off]->iter = f(&chunk->borders[i - off]->z,
					chunk->borders[i - off]->z, env->iter);
		}
	}
	return (0);
}

//handle size < 2
int	boundary_trace_fractal_r(t_env *env, t_chunk *chunk,
	size_t (*f)(t_complex *, t_complex, size_t))
{
	int	dir;
	int	i;
	int	end;
	int	iter;

	if (iterate_chunk_borders(env, chunk, f))
		return (1);
	dir = 0;
	iter = chunk->borders[0][0].iter;
	while (dir < 4)
	{
		i = 0;
		end = chunk->bounds[2 + ((dir + 1) % 2)] - chunk->bounds[(dir + 1) % 2];
		if (end < 2)
			return (color_chunk(env->frame, chunk), 0);
		while (i < end)
		{
			if (iter != chunk->borders[dir][i].iter)
				;//return SUBDIVIDE
			i++;
		}
	}
	color_chunk(env->frame, chunk);
	return (0);
}

int	boundary_trace_fractal(t_env *env,
	size_t (*f)(t_complex *, t_complex, size_t))
{
	t_chunk	chunk;
	int		i;

	i = 0;
	while (i < 4)
		chunk.borders[i++] = 0;
	chunk.bounds[UP] = 0;
	chunk.bounds[LEFT] = 0;
	chunk.bounds[DOWN] = env->frame->height - 1;
	chunk.bounds[RIGHT] = env->frame->width - 1;
	chunk.filled = 0;
	chunk.subdivisions = 0;
	return (boundary_trace_fractal_r(env, &chunk, f));
}

size_t	mandelbrot_iterate(t_complex *z, t_complex c, size_t iterations)
{
	size_t		iter;
	t_complex	num;

	num = *z;
	iter = iterations;
	while (iter)
	{
		num = add_complex(square_complex(num), c);
		if (dist_origin_squared(num) > 4)
			return (*z = num, iterations - iter);
		iter--;
	}
	return (*z = num, iterations);
}
