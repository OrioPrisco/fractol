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
#include <stdio.h>

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

//TODO : determine how many iterations are stricly required for crtain scale
//       and how many are recommended. Then increase iterations 
//       in the background when you have nothing to do

t_complex	calculate_top_left(t_env *env)
{
	t_complex	top_left;

	top_left.real = (env->camera_center.real
			- ((env->frame->width / env->scale) / 2));
	top_left.imag = (env->camera_center.imag
			- ((env->frame->height / env->scale) / 2));
	return (top_left);
}

size_t	mandelbrot_iterate(t_complex *z, t_complex c, size_t iterations);
int	boundary_trace_fractal(t_env *env,
	size_t (*f)(t_complex *, t_complex, size_t));
void	draw(t_env *env)
{

	printf("hello from draw\n");
	boundary_trace_fractal(env, mandelbrot_iterate);
	mlx_put_image_to_window(env->mlx, env->win, env->frame->img, 0, 0);
}

void	free_lines(t_iter_result *borders[4], char filled)
{
	int	dir;

	dir = 0;
	while (dir < 4)
	{
		if (filled & 1 << dir++)
		{
			free(borders[dir]);
		}
	}
}

void	color_uniform_chunk(t_img *img, t_chunk *chunk, int iter)
{
	int	x;
	int	y;
	int	color;

	y = chunk->top_left[1];
	color = chunk->borders[0][0].iter;
	color = (color == iter)? 0 : g_palette2[color % 9];
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

void	color_small_chunk(t_img *img, t_chunk *chunk, int iter, t_env* env)
{
	int	x;
	int	y;
	int	color;

	(void)env;
	y = chunk->top_left[1];
	color = chunk->borders[0][0].iter;
	color = (color == iter)? 0 : g_palette2[color % 9];
	while (y < chunk->top_left[1] + chunk->dimensions[1])
	{
		x = chunk->top_left[0];
		while (x < chunk->top_left[0] + chunk->dimensions[0])
		{
			if (y == chunk->top_left[1])
			{
				color = chunk->borders[UP][x - chunk->top_left[0]].iter;
				color = (color == iter)? 0 : g_palette2[color % 9];
			}
			else if (x == chunk->top_left[0]) {
				color = chunk->borders[LEFT][(y - chunk->top_left[1])].iter;
				color = (color == iter)? 0 : g_palette2[color % 9];
			}
			my_mlx_pixel_put(img, x, y, color);
			x++;
		}
		y++;
	}
}

int	iterate_chunk_borders(t_env *env, t_chunk *chunk,
	size_t (*f)(t_complex *, t_complex, size_t))
{
	t_direction	dir;
	t_complex	top_left;
	int			i;

	dir = -1;
	top_left = add_complex(
		calculate_top_left(env),
		complex(
			chunk->top_left[0] / env->scale,
			chunk->top_left[1] / env->scale
		)
	);
	while (++dir < 4)
	{
		if (chunk->filled & (1 << dir))
			continue ;
		i = 0;
		while (i < chunk->dimensions[dir % 2])
		{
			chunk->borders[dir][i].z = add_complex(
			complex(
			((dir == 3) * chunk->dimensions[0] + i * (!(dir % 2))) / env->scale,
			((dir == D) * chunk->dimensions[1] + i * (dir % 2)) / env->scale),
				top_left
			);
			chunk->borders[dir][i].c = chunk->borders[dir][i].z;
			chunk->borders[dir][i].iter = f(&chunk->borders[dir][i].z,
				chunk->borders[dir][i].z, env->iter);
			i++;
		}
	}
	return (0);
}


int	boundary_trace_fractal_r(t_env *env, t_chunk *chunk,
	size_t (*f)(t_complex *, t_complex, size_t));

int	subdivide_chunk(t_env *env, t_chunk *chunk,
	size_t (*f)(t_complex *, t_complex, size_t))
{
	t_chunk			c1;
	t_chunk			c2;
	int				h_split;
	t_iter_result	shared_border[WIDTH+HEIGHT];

	c1 = *chunk;
	c2 = *chunk;
	h_split = (!(chunk->filled & 1 << UP) || !(chunk->filled & 1 << DOWN));
	c1.filled = ~(1 << (LEFT - h_split) % 4);
	c2.filled = ~0;
	c1.top_left[h_split] += chunk->dimensions[h_split] / 2;
	c1.dimensions[h_split] -= chunk->dimensions[h_split] / 2;
	c2.dimensions[h_split] -= c1.dimensions[h_split];
	c1.borders[(UP + h_split) % 4] += c2.dimensions[h_split];
	c1.borders[(DOWN + h_split) % 4] += c2.dimensions[h_split];
	c1.borders[(LEFT - h_split) % 4] = shared_border;
	c2.borders[(RIGHT - h_split) % 4] = shared_border;
	boundary_trace_fractal_r(env, &c1, f);
	boundary_trace_fractal_r(env, &c2, f);
	return (0);
}

int	boundary_trace_fractal_r(t_env *env, t_chunk *chunk,
	size_t (*f)(t_complex *, t_complex, size_t))
{
	int	dir;
	int	i;

	if (iterate_chunk_borders(env, chunk, f))
		return (1);
	dir = -1;
	if (chunk->dimensions[0] < 2 || chunk->dimensions[1] < 2)
		return (color_small_chunk(env->frame, chunk, env->iter, env), 0);
	while (++dir < 4)
	{
		i = 0;
		while (i < chunk->dimensions[dir % 2])
		{
			if (chunk->borders[0][0].iter != chunk->borders[dir][i].iter)
				return (subdivide_chunk(env, chunk, f));
			i++;
		}
	}
	return (color_uniform_chunk(env->frame, chunk, env->iter), 0);
}

int	boundary_trace_fractal(t_env *env,
	size_t (*f)(t_complex *, t_complex, size_t))
{
	t_chunk		chnk;
	t_iter_result	borders[4][WIDTH+HEIGHT];

	chnk.top_left[0] = 0;
	chnk.top_left[1] = 0;
	chnk.dimensions[0] = env->frame->width;
	chnk.dimensions[1] = env->frame->height;
	chnk.borders[UP] = borders[UP];
	chnk.borders[LEFT] = borders[LEFT];
	chnk.borders[DOWN] = borders[DOWN];
	chnk.borders[RIGHT] = borders[RIGHT];
	chnk.filled = 0;
	return (boundary_trace_fractal_r(env, &chnk, f));
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
