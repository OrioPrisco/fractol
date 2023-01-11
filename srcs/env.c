/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: OrioPrisco <47635210+OrioPrisco@users      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 15:29:17 by OrioPrisc         #+#    #+#             */
/*   Updated: 2022/12/09 18:27:37 by OrioPrisc        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "fractol.h"
#include "libft.h"

static const int			g_palette[] = {
	0x0000ff,
	0x00ff00,
	0x000f60,
	0xff0000,
	0x0f6000
};

static const int			g_palette2[] = {
	0xb40982,
	0x4e54cb,
	0x4e94cb,
	0x4edfcb,
	0x4edf00,
	0xb1df00,
	0xffdf00,
	0xff9a00,
	0xff6b00,
	0xff5700
};

static const int			g_palette3[] = {
	0xB40982,
	0xA91089,
	0x9F1890,
	0x951F97,
	0x8B279F,
	0x812EA6,
	0x7636AD,
	0x6C3DB5,
	0x6245BC,
	0x584CC3,
	0x4E54CB,
	0x4E5ACB,
	0x4E60CB,
	0x4E67CB,
	0x4E6DCB,
	0x4E74CB,
	0x4E7ACB,
	0x4E80CB,
	0x4E87CB,
	0x4E8DCB,
	0x4E94CB,
	0x4E9BCB,
	0x4EA3CB,
	0x4EAACB,
	0x4EB2CB,
	0x4EB9CB,
	0x4EC1CB,
	0x4EC8CB,
	0x4ED0CB,
	0x4ED7CB,
	0x4EDFCB,
	0x4EDFB6,
	0x4EDFA2,
	0x4EDF8E,
	0x4EDF79,
	0x4EDF65,
	0x4EDF51,
	0x4EDF3C,
	0x4EDF28,
	0x4EDF14,
	0x4EDF00,
	0x57DF00,
	0x61DF00,
	0x6BDF00,
	0x75DF00,
	0x7FDF00,
	0x89DF00,
	0x93DF00,
	0x9DDF00,
	0xA7DF00,
	0xB1DF00,
	0xB8DF00,
	0xC0DF00,
	0xC8DF00,
	0xD0DF00,
	0xD8DF00,
	0xDFDF00,
	0xE7DF00,
	0xEFDF00,
	0xF7DF00,
	0xFFDF00,
	0xFFD800,
	0xFFD100,
	0xFFCA00,
	0xFFC300,
	0xFFBC00,
	0xFFB500,
	0xFFAE00,
	0xFFA700,
	0xFFA000,
	0xFF9A00,
	0xFF9500,
	0xFF9000,
	0xFF8B00,
	0xFF8700,
	0xFF8200,
	0xFF7D00,
	0xFF7900,
	0xFF7400,
	0xFF6F00,
	0xFF6B00,
	0xFF6900,
	0xFF6700,
	0xFF6500,
	0xFF6300,
	0xFF6100,
	0xFF5F00,
	0xFF5D00,
	0xFF5B00,
	0xFF5900,
	0xFF5700,
	0xF74F0D,
	0xF0471A,
	0xE83F27,
	0xE13734,
	0xD93041,
	0xD2284E,
	0xCA205B,
	0xC31868,
	0xBB1075,
};

static const int			g_palette4[] = {
	0xFFBA00,
	0xFECF00,
	0xFEE500,
	0xFEFB00,
	0xEBFE00,
	0xD5FE00,
	0xBFFE00,
	0xA9FE00,
	0x93FE00,
	0x7DFE00,
	0x67FE00,
	0x51FE00,
	0x3BFE00,
	0x25FD00,
	0x0FFD00,
	0x00FD05,
	0x00FD1B,
	0x00FD31,
	0x00FD47,
	0x00FD5D,
	0x00FD73,
	0x00FD89,
	0x00FD9E,
	0x00FDB4,
	0x00FDCA,
	0x00FDE0,
	0x00FCF5,
	0x00EEFC,
	0x00D8FC,
	0x00C2FC,
	0x00ACFC,
	0x0096FC,
	0x0080FC,
	0x006AFC,
	0x0055FC,
	0x003FFC,
	0x0029FC,
	0x0013FC,
	0x0100FB,
	0x1700FB,
	0x2D00FB,
	0x4300FB,
	0x5800FB,
	0x6E00FB,
	0x8400FB,
	0x9900FB,
	0xAF00FB,
	0xC400FB,
	0xDA00FB,
	0xF000FB,
	0xFB00F0,
	0xFA00DA,
	0xFA00C4,
	0xFA00AF,
	0xFA0099,
	0xFA0083,
	0xFA006E,
	0xFA0058,
	0xFA0042,
	0xFA002D,
	0xFA0017,
	0xFA0002,
	0xFA1300,
	0xF92800,
	0xF93C00,
	0xFA5100,
	0xFB6500,
	0xFC7A00,
	0xFD8F00,
	0xFEA400,
	0xFFBA00,
};
static const t_palette_data	g_palettes[] = {
{g_palette, sizeof(g_palette) / sizeof(g_palette[0])},
{g_palette2, sizeof(g_palette2) / sizeof(g_palette2[0])},
{g_palette3, sizeof(g_palette3) / sizeof(g_palette3[0])},
{g_palette4, sizeof(g_palette4) / sizeof(g_palette4[0])},
};

static t_f_iterator *const	g_fractals[] = {
	mandelbrot_iterate,
	julia_iterate,
	burning_ship_iterate,
	julia_ship_iterate,
};

static int	init_img(t_env *env, t_img *img, int width, int height)
{
	img->img = 0;
	img->img = mlx_new_image(env->mlx, width, height);
	if (!img->img)
		return (1);
	img->data = (unsigned char *)mlx_get_data_addr(img->img,
			&img->bits_per_pixel, &img->line_length, &img->endian);
	img->height = height;
	img->width = width;
	return (0);
}

int	init_env(t_env *env)
{
	env->mlx = mlx_init();
	env->frame = &env->frame1;
	if (!env->mlx)
		return (1);
	env->win = mlx_new_window(env->mlx, WIDTH, HEIGHT, "Fractol");
	if (!env->win)
		return (2);
	env->camera.palette.data = &g_palettes[2];
	env->camera.iter = 10;
	env->camera.scale = 400;
	env->camera.center = complex(0, 0);
	if (init_img(env, &env->frame1, WIDTH, HEIGHT)
		|| init_img(env, &env->frame2, WIDTH, HEIGHT)
		|| init_img(env, &env->camera.work_buffer, WIDTH, HEIGHT))
		return (1);
	update_camera(&env->camera);
	return (0);
}

void	next_palette(t_palette *palette)
{
	size_t	i;

	i = palette->data - &g_palettes[0];
	i++;
	if (i >= (sizeof(g_palettes) / sizeof(g_palettes[0])))
		i = 0;
	palette->data = &g_palettes[i];
}

void	switch_frame(t_env *env)
{
	if (env->frame == &env->frame1)
		env->frame = &env->frame2;
	else
		env->frame = &env->frame1;
}

void	switch_fractal(t_f_iterator **iterator)
{
	size_t	i;

	i = 0;
	while (++i < (sizeof(g_fractals) / sizeof(g_fractals[0])))
		if (*iterator == g_fractals[i - 1])
			break ;
	if (i >= (sizeof(g_fractals) / sizeof(g_fractals[0])))
		i = 0;
	*iterator = g_fractals[i];
}
