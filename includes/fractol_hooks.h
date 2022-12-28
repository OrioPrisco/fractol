/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_hooks.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:02:47 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/28 12:42:15 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_HOOKS_H
# define FRACTOL_HOOKS_H

struct					s_env;
typedef struct s_env	t_env;

int		quit_prg(t_env *env);
int		deal_key(int key, t_env *env);
int		my_mouse_hook(int button, int x, int y, t_env *env);
int		my_expose(t_env *env);
void	draw(t_env *env);

#endif
