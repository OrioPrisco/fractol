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

typedef double			t_complex_part;

typedef struct s_complex
{
	t_complex_part	real;
	t_complex_part	imag;
}	t_complex;

struct					s_env;
typedef struct s_env	t_env;

t_complex_part		dist_origin_squared(t_complex num);
t_complex			add_complex(t_complex c1, t_complex c2);
t_complex			square_complex(t_complex num);
t_complex			calculate_top_left(t_env *env);
t_complex			complex(t_complex_part real, t_complex_part imag);
#endif
