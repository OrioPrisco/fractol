/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 12:57:29 by OrioPrisc         #+#    #+#             */
/*   Updated: 2023/01/10 16:39:17 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "fractol.h"
#include "fractals.h"
#include "parse.h"

static void	print_usage(t_parse_error error)
{
	if (error == NO_FRACTAL_NAME)
		ft_printf("Error : No Name Provided\n");
	if (error == BAD_FRACTAL_NAME)
		ft_printf("Error : Bad Fractal Name\n");
	if (error == NO_JULIA_PARAMS)
		ft_printf("Error : Julia requires 2 parameters\n");
	if (error == BAD_JULIA_PARAMS)
		ft_printf("Error : Arguments for Julia have to be doubles\n");
	ft_printf
		("Usage : ./fractol <mandelbrot|julia|ship|julia_ship> <params>\n");
}

int	parse(int argc, char **argv, t_env *env)
{
	if (argc < 2)
		return (print_usage(NO_FRACTAL_NAME), 1);
	if (assign_fractal(argv[1], &env->fractal))
		return (print_usage(BAD_FRACTAL_NAME), 1);
	if (!ft_strncmp("julia", argv[1], ft_strlen("julia")))
	{
		if (argc < 4)
			return (print_usage(NO_JULIA_PARAMS), 1);
		if (ft_atod(argv[2], &env->camera.params.julia_c.real)
			|| ft_atod(argv[3], &env->camera.params.julia_c.imag))
			return (print_usage(BAD_JULIA_PARAMS), 1);
	}
	return (0);
}
