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
	env->win = 0;
	env->mlx = mlx_init();
	env->frame1.img = 0;
	env->frame2.img = 0;
	env->frame = &env->frame1;
	env->camera.work_buffer.img = 0;
	if (!env->mlx)
		return (1);
	env->win = mlx_new_window(env->mlx, WIDTH, HEIGHT, "Fractol");
	if (!env->win)
		return (2);
	env->camera.iter = 10;
	env->camera.scale = 400;
	env->camera.center = complex(0, 0);
	env->camera.debug = 0;
	if (init_img(env, &env->frame1, WIDTH, HEIGHT)
		||init_img(env, &env->frame2, WIDTH, HEIGHT)
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
