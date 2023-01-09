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
# include "fractol_image.h"
# include "camera.h"
# include "fractals.h"

typedef struct s_env
{
	void			*win;
	void			*mlx;
	t_img			*frame;
	t_img			frame1;
	t_img			frame2;
	t_camera		camera;
	t_complex		julia_c;
	t_f_iterator	*iterator;
}	t_env;

int		init_env(t_env *env);
void	switch_frame(t_env *env);
void	next_palette(t_palette *palette);
void	switch_fractal(t_f_iterator **env);

#endif
