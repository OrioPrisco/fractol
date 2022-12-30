/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractals.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:53:38 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/28 12:54:50 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTALS_H
# define FRACTALS_H

//the definition of t_complex and t_camera are needed
# include "complex.h"
# include "fractol.h"

typedef size_t	t_fractal_iterator (t_complex *, t_complex, size_t, void *);

size_t	mandelbrot_iterate(t_complex *z, t_complex c, size_t iterations,
			void *data);
int		boundary_trace_fractal(t_camera *camera,
			t_fractal_iterator *f, void *data);

#endif
