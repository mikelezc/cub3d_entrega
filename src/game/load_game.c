/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 19:27:29 by mlezcano          #+#    #+#             */
/*   Updated: 2024/07/29 17:16:10 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	cb_paint_floor_ceiling(t_game *game, t_parse *data)
{
	game->map.ceil_color = ((data->c_color[0] & 0xFF) << 16) | \
	((data->c_color[1] & 0xFF) << 8) | \
	(data->c_color[2] & 0xFF);
	game->map.floor_color = ((data->f_color[0] & 0xFF) << 16) | \
	((data->f_color[1] & 0xFF) << 8) | \
	(data->f_color[2] & 0xFF);
}

void	cb_load_texture(t_game *game, int *texture, char *path, t_img *img)
{
	int	y;
	int	x;

	y = 0;
	img->img = mlx_xpm_file_to_image(game->mlx, path,
			&img->img_width, &img->img_height);
	if (!img->img)
		cb_error_exit("Error loading texture");
	img->data = (int *)mlx_get_data_addr(img->img,
			&img->bpp, &img->size_l, &img->endian);
	if (!img->data || img->img_width != TEXWIDTH \
		|| img->img_height != TEXHEIGHT)
		cb_error_exit("Error texture size or format");
	while (y < img->img_height)
	{
		x = 0;
		while (x < img->img_width)
		{
			texture[img->img_width * y + x] = img->data[img->img_width * y + x];
			x++;
		}
		y++;
	}
	mlx_destroy_image(game->mlx, img->img);
}

void	cb_load_img_wall(t_game *game, t_parse *data)
{
	t_img	img;

	cb_load_texture(game, game->texture[0], data->so, &img);
	cb_load_texture(game, game->texture[1], data->no, &img);
	cb_load_texture(game, game->texture[2], data->we, &img);
	cb_load_texture(game, game->texture[3], data->ea, &img);
	free(data->no);
	free(data->so);
	free(data->ea);
	free(data->we);
}

void	cb_set_img_wall(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	game->texture = ft_calloc(5, sizeof(int *));
	if (!game->texture)
		cb_error_exit("Error inicializating textures");
	while (i < 4)
	{
		game->texture[i] = ft_calloc(TEXHEIGHT * TEXWIDTH + 1, sizeof(int));
		if (!game->texture[i++])
			cb_error_exit("Error inicializating textures");
	}
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < TEXHEIGHT * TEXWIDTH)
		{
			game->texture[i][j] = 0;
			j++;
		}
		i++;
	}
}

void	cb_load_game(t_game *game, t_parse *data)
{
	cb_set_img_wall(game);
	cb_load_img_wall(game, data);
	cb_paint_floor_ceiling(game, data);
}
