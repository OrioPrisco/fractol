/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 15:29:17 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/09 18:27:37 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "fractol.h"
#include "libft.h"

/*
static t_f_iterator *const	g_fractals[] = {
	mandelbrot_iterate,
	julia_iterate,
	burning_ship_iterate,
	julia_ship_iterate,
};
*/

static const t_fractal	g_julia = {
	julia_iterate,
	NULL,
};
static const t_fractal	g_mandelbrot = {
	mandelbrot_iterate,
	&g_julia,
};
static const t_fractal	g_julia_ship = {
	julia_ship_iterate,
	NULL,
};
static const t_fractal	g_burning_ship = {
	burning_ship_iterate,
	&g_julia_ship,
};

static const t_fractal	*g_fractals[] = {
	&g_mandelbrot,
	&g_julia,
	&g_burning_ship,
	&g_julia_ship,
};

bool	assign_fractal(const char *name, const t_fractal **fractal)
{
	if (!ft_strcmp(name, "mandelbrot"))
		*fractal = g_fractals[0];
	else if (!ft_strcmp(name, "julia"))
		*fractal = g_fractals[1];
	else if (!ft_strcmp(name, "ship"))
		*fractal = g_fractals[2];
	else if (!ft_strcmp(name, "julia_ship"))
		*fractal = g_fractals[3];
	else
		return (1);
	return (0);
}

static int	init_img(t_env *env, t_img *img, int width, int height)
{
	img->img = 0;
	img->img = mlx_new_image(env->mlx, width, height);
	if (!img->img)
		return (1);
	img->data = (unsigned char *)mlx_get_data_addr(img->img,
			&img->bits_per_pixel, &img->line_length, &img->endian);
	img->height = height;
	img->width = width;
	return (0);
}

int	init_env(t_env *env)
{
	env->mlx = mlx_init();
	env->frame = &env->frame1;
	if (!env->mlx)
		return (1);
	env->win = mlx_new_window(env->mlx, WIDTH, HEIGHT, "Fractol");
	if (!env->win)
		return (2);
	default_palette(&env->camera.palette);
	env->camera.iter = 10;
	env->camera.scale = 400;
	env->camera.center = complex(0, 0);
	if (init_img(env, &env->frame1, WIDTH, HEIGHT)
		|| init_img(env, &env->frame2, WIDTH, HEIGHT)
		|| init_img(env, &env->camera.work_buffer, WIDTH, HEIGHT))
		return (1);
	update_camera(&env->camera);
	return (0);
}

void	switch_frame(t_env *env)
{
	if (env->frame == &env->frame1)
		env->frame = &env->frame2;
	else
		env->frame = &env->frame1;
}

void	switch_fractal(const t_fractal **fractal)
{
	size_t	i;

	i = 0;
	while (++i < (sizeof(g_fractals) / sizeof(g_fractals[0])))
		if (*fractal == g_fractals[i - 1])
			break ;
	if (i >= (sizeof(g_fractals) / sizeof(g_fractals[0])))
		i = 0;
	*fractal = g_fractals[i];
}
