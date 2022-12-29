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

int	init_env(t_env *env, t_img *img)
{
	t_img	*cam_img;

	env->win = 0;
	env->mlx = mlx_init();
	img->img = 0;
	env->camera.work_buffer.img = 0;
	if (!env->mlx)
		return (1);
	env->win = mlx_new_window(env->mlx, WIDTH, HEIGHT, "Fractol");
	env->frame = img;
	if (!env->win)
		return (2);
	env->camera.iter = 3;
	env->camera.scale = 400;
	env->camera.center = complex(0,0);
	env->debug = 0;
	env->camera.work_buffer.img = mlx_new_image(env->mlx, WIDTH, HEIGHT);
	img->img = mlx_new_image(env->mlx, WIDTH, HEIGHT);
	if (!img->img || ! env->camera.work_buffer.img)
		return (1);
	img->data = (unsigned char *)mlx_get_data_addr(img->img,
			&img->bits_per_pixel, &img->line_length, &img->endian);
	cam_img = &env->camera.work_buffer;
	cam_img->data = (unsigned char *)mlx_get_data_addr(cam_img->img,
			&cam_img->bits_per_pixel, &cam_img->line_length, &cam_img->endian);
	img->height = HEIGHT;
	img->width = WIDTH;
	cam_img->width = WIDTH;
	cam_img->height = HEIGHT;
	update_camera(&env->camera);
	return (0);
}
