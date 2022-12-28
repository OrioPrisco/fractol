/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_chunk.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:42:45 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/28 12:52:21 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_CHUNK_H
# define COLOR_CHUNK_H

struct					s_img;
typedef struct s_img	t_img;

typedef enum e_direction {
	UP = 0,
	LEFT = 1,
	DOWN = 2,
	RIGHT = 3,
	U = UP,
	L = LEFT,
	D = DOWN,
	R = RIGHT,
}	t_direction;

typedef struct s_iter_result
{
	t_complex	z;
	t_complex	c;
	int			iter;
}	t_iter_result;

typedef struct s_chunk
{
	t_iter_result	*borders[4];
	int				top_left[2];
	int				dimensions[2];
	char			filled;

}	t_chunk;

void	my_mlx_pixel_put(t_img *img, int x, int y, int color);
void	color_uniform_chunk(t_img *img, t_chunk *chunk, int iter);
void	color_small_chunk(t_img *img, t_chunk *chunk, int iter);

#endif
