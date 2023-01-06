/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:43:11 by OrioPrisc         #+#    #+#             */
/*   Updated: 2023/01/06 17:50:46 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H
# include "complex.h"
# include "fractol_image.h"
# include <stddef.h>

struct					s_chunk;
typedef struct s_chunk	t_chunk;

typedef enum e_debug
{
	DBG_WINDING = 1 << 0,
	DBG_CHUNK_BORDERS = 1 << 1,
	DBG_WINDING_STEP = 1 << 2,
	DBG_SHOW_NO_DEEPEN = 1 << 3,
}	t_debug;

typedef struct s_camera
{
	size_t		iter;
	long double	scale;
	t_complex	center;
	t_complex	top_left;
	t_complex	step;
	t_img		work_buffer;
	t_debug		debug;
	t_chunk		*chunk;
}	t_camera;

void	update_camera(t_camera *camera);
void	zoom_camera(t_camera *camera, double scale);
void	move_camera(t_camera *camera, t_complex movement);

#endif
