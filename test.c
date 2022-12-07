/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 16:09:12 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/05 16:22:58 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <unistd.h>
#include <X11/X.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 601
#define HEIGHT 500
#define MIN(A,B) (((A)>(B))?(B):(A))

typedef struct s_img
{
	void	*img;
	char	*data;
	int		line_length;
	int		bits_per_pixel;
	int		endian;
	int		height;
	int		width;
}	t_img;

typedef struct s_env
{
	void	*win;
	void	*mlx;
	t_img	*frame;
}	t_env;

int	quit_prg(t_env *env)
{
	mlx_destroy_window(env->mlx, env->win);
	mlx_destroy_display(env->mlx);
//	free(env->mlx);
	exit(0);
}

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	unsigned char	*dest;
	
	dest = img->data + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int*)dest = color;
}

int	deal_key(int key, void *param)
{
	(void)param;
	write(1, &key, 1);
	
	if (key == 65307)
		quit_prg(param);
	return (0);
}

double	dist(int x, int y, int x2, int y2)
{
	double	dx;
	double	dy;

	dx = x2 - x;
	if (dx < 0)
		dx = -dx;
	dy = y2 - y;
	if (dy < 0)
		dy = -dy;
	return (sqrt((dx * dx) + (dy * dy)));
}

int dist2(int x, int y, int x2, int y2, t_img *img)
{

	return (1 - (int)(MIN(dist(x, y, x2, y2),
		dist(x2, y2, img->width / 2, img->height / 3)))
		/ dist(x2, y2, img->width / 2, img->height / 3)) * 0xFF;
}
int	my_loop(t_env *env)
{
	int			color;

	for (int x = 0; x < env->frame->width; x++)
	{
	for (int y = 0; y < env->frame->height; y++)
	{
	color =  dist2(x, y, 0, 0, env->frame) << 16;
	color += dist2(x, y, env->frame->width, 0, env->frame) << 8;
	color += dist2(x, y, env->frame->width / 2, env-> frame->height, env->frame) << 0;
	my_mlx_pixel_put(env->frame, x, y, color);
	}}
	mlx_put_image_to_window(env->mlx, env->win, env->frame->img, 0, 0);
}

int main()
{
	t_env env;
	t_img img;

	env.mlx = mlx_init();
	env.frame = &img;
	if (!env.mlx)
		return (1);
	env.win = mlx_new_window(env.mlx, WIDTH, HEIGHT, "Foo bar");
	if (!env.win)
		return (2);
	img.img = mlx_new_image(env.mlx, WIDTH, HEIGHT);
	img.data = mlx_get_data_addr(img.img, &img.bits_per_pixel,
		&img.line_length, &img.endian);
	img.height = HEIGHT;
	img.width = WIDTH;
	printf("image %p\n", img.img);
	mlx_key_hook(env.win, &deal_key, &env);
	mlx_loop_hook(env.mlx, my_loop, &env);
	mlx_expose_hook(env.win, my_loop, &env);
	mlx_hook(env.win, DestroyNotify, StructureNotifyMask , &quit_prg, &env);
	mlx_loop(env.mlx);
}
