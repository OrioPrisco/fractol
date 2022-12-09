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
#include "mlx.h"

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

void	draw(t_env *env)
{
	int			x;
	int			y;
	int			iter;
	t_complex	c;
	t_complex	top_left;

	top_left.real = (env->camera_center.real
		- ((env->frame->width / env->scale) / 2));
	top_left.imag = (env->camera_center.imag
		- ((env->frame->height / env->scale) / 2));
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

size_t mandelbrot_iterate(t_complex *z, t_complex c, size_t iterations)
{
	size_t		iter;
	t_complex	num;

	num = *z;
	iter = iterations;
	while(iter)
	{
		num = add_complex(square_complex(num), c);
		if (dist_origin_squared(num) > 4)
			return (*z = num, iterations - iter);
		iter--;
	}
	return (*z = num, iterations);
}
