/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:02:53 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/29 19:51:39 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	update_camera(t_camera *camera)
{
	camera->top_left.real = (camera->center.real
			- ((camera->work_buffer.width / camera->scale) / 2));
	camera->top_left.imag = (camera->center.imag
			- ((camera->work_buffer.height / camera->scale) / 2));
	camera->step.real = (1 / camera->scale);
	camera->step.imag = (1 / camera->scale);
}

void	zoom_camera(t_camera *camera, double scale)
{
	camera->scale *= scale;
	update_camera(camera);
}

void	move_camera(t_camera *camera, t_complex movement)
{
	movement.real *= camera->work_buffer.width * camera->step.real;
	movement.imag *= camera->work_buffer.height * camera->step.imag;
	camera->center = add_complex(camera->center, movement);
	update_camera(camera);
}
