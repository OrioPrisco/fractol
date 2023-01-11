/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deal_key.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 17:09:51 by OrioPrisc         #+#    #+#             */
/*   Updated: 2023/01/03 17:13:19 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_hooks.h"
#include "fractol.h"
#include <X11/X.h>
#include <X11/keysym.h>
#include "complex.h"

static void	deal_key_dbg(int key, t_env *env)
{
	if (key == XK_F1)
		return (invalidate_chunks(&env->camera),
			(void)(env->camera.debug = env->camera.debug ^ DBG_WINDING));
	if (key == XK_F2)
		env->camera.debug = env->camera.debug ^ DBG_CHUNK_BORDERS;
	if (key == XK_F3)
		env->camera.debug = env->camera.debug ^ DBG_WINDING_STEP;
	if (key == XK_F4)
		env->camera.debug = env->camera.debug ^ DBG_SHOW_NO_DEEPEN;
	if (key == XK_F5)
		next_palette(&env->camera.palette);
	if (key == XK_F6)
		env->camera.palette.color_shift ++;
	if (key == XK_F7)
		return (invalidate_chunks(&env->camera),
			switch_fractal(&env->iterator));
}

int	deal_key(int key, t_env *env)
{
	if (key == XK_p && env->camera.debug & DBG_WINDING)
		env->camera.iter++;
	if (key == XK_m && env->camera.debug & DBG_WINDING)
		env->camera.iter--;
	if (key == XK_8)
		zoom_camera(&env->camera, 1.1);
	if (key == XK_slash)
		zoom_camera(&env->camera, 0.9);
	if (key == XK_w || key == XK_Up)
		move_camera(&env->camera, complex(0, -0.05));
	if (key == XK_s || key == XK_Down)
		move_camera(&env->camera, complex(0, 0.05));
	if (key == XK_a || key == XK_Left)
		move_camera(&env->camera, complex(-0.05, 0));
	if (key == XK_d || key == XK_Right)
		move_camera(&env->camera, complex(0.05, 0));
	if (key == XK_Escape)
		quit_prg(env);
	deal_key_dbg(key, env);
	draw(env);
	return (0);
}
