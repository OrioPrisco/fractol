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

static const int			g_palette[] = {
	0x0000ff,
	0x00ff00,
	0x000f60,
	0xff0000,
	0x0f6000
};

static const int			g_palette2[] = {
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

static const t_palette_data	g_palettes[] = {
{g_palette, sizeof(g_palette) / sizeof(g_palette[0])},
{g_palette2, sizeof(g_palette2) / sizeof(g_palette2[0])},
};

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
	ft_bzero(env, sizeof(*env));
	env->mlx = mlx_init();
	env->frame = &env->frame1;
	if (!env->mlx)
		return (1);
	env->win = mlx_new_window(env->mlx, WIDTH, HEIGHT, "Fractol");
	if (!env->win)
		return (2);
	env->camera.palette.data = &g_palettes[1];
	env->camera.iter = 10;
	env->camera.scale = 400;
	env->camera.center = complex(0, 0);
	if (init_img(env, &env->frame1, WIDTH, HEIGHT)
		||init_img(env, &env->frame2, WIDTH, HEIGHT)
		|| init_img(env, &env->camera.work_buffer, WIDTH, HEIGHT))
		return (1);
	update_camera(&env->camera);
	return (0);
}

void	next_palette(t_palette *palette)
{
	size_t	i;

	i = g_palettes - palette->data;
	i++;
	if (i >= (sizeof(g_palettes) / sizeof(g_palettes[0])))
		i = 0;
	palette->data = &g_palettes[i];
}

void	switch_frame(t_env *env)
{
	if (env->frame == &env->frame1)
		env->frame = &env->frame2;
	else
		env->frame = &env->frame1;
}
