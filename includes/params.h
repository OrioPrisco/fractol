/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   params.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 19:38:35 by OrioPrisc         #+#    #+#             */
/*   Updated: 2023/01/11 19:39:31 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAMS_H
# define PARAMS_H

# include "complex.h"
# include <stddef.h>

typedef struct s_param
{
	t_complex	julia_c;
	size_t		mandelbrot_heads;
}	t_param;

#endif
