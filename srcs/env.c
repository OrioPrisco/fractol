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
	env->win = 0;
	env->mlx = mlx_init();
	img->img = 0;
	if (!env->mlx)
		return (1);
	env->win = mlx_new_window(env->mlx, WIDTH, HEIGHT, "Fractol");
	env->frame = img;
	if (!env->win)
		return (2);
	env->iter = 1;
	env->scale = 200;
	env->camera_center.real = 0;
	env->camera_center.imag = 0;
	img->img = mlx_new_image(env->mlx, WIDTH, HEIGHT);
	img->data = (unsigned char *)mlx_get_data_addr(img->img,
			&img->bits_per_pixel, &img->line_length, &img->endian);
	img->height = HEIGHT;
	img->width = WIDTH;
	return (0);
}
