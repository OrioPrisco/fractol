/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 13:02:48 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/28 13:08:08 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPLEX_H
# define COMPLEX_H

typedef struct s_complex
{
	double	real;
	double	imag;
}	t_complex;

struct					s_env;
typedef struct s_env	t_env;

double		dist_origin_squared(t_complex num);
t_complex	add_complex(t_complex c1, t_complex c2);
t_complex	square_complex(t_complex num);
t_complex	calculate_top_left(t_env *env);
t_complex	complex(double real, double imag);
#endif
