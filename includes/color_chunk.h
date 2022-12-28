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
struct					s_chunk;
typedef struct s_chunk	t_chunk;
typedef struct s_img	t_img;

void	color_uniform_chunk(t_img *img, t_chunk *chunk, int iter);
void	color_small_chunk(t_img *img, t_chunk *chunk, int iter);

#endif
