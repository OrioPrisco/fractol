/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 15:33:21 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/08 15:58:10 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# define WIDTH 800
# define HEIGHT 800
# include <stddef.h>
# include "complex.h"
# include "fractol_image.h"

typedef struct s_camera
{
	int			iter;
	long double	scale;
	t_complex	center;
	t_complex	top_left;
	t_complex	step;
	t_img		work_buffer;
}	t_camera;

typedef struct s_env
{
	void			*win;
	void			*mlx;
	t_img			*frame;
	t_img			frame1;
	t_img			frame2;
	t_camera		camera;
	char			debug;
}	t_env;

int		init_env(t_env *env);
void	switch_frame(t_env *env);
void	update_camera(t_camera *camera);
void	zoom_camera(t_camera *camera, double scale);
void	move_camera(t_camera *camera, t_complex movement);

#endif
