#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct	s_background
{
	int			w;
	int			h;
	char		c;
}				t_background;

typedef struct	s_rectangle
{
	char		t;
	float		x;
	float		y;
	float		w;
	float		h;
	char		c;
}				t_rectangle;

int		clear_all(FILE *f, char *z)
{
	fclose(f);
	if(z)
		free(z);
	return (1);
}

int		str_error(char *s)
{
	int		i;

	i = 0;
	while (s[i])
		i++;
	write(1, s, i);
	return (1);
}

int		main(int ac, char **av)
{
	FILE			*f;
	t_background	bg;
	t_rectangle		r;
	char			*z;
	char			cr;
	float			x;
	float			y;
	int				i;
	int				j;
	int				sr;

	if (ac != (2))
		return (str_error("Error: argument.\n"));
	if (!(f = fopen(av[1], "r")))
		return (str_error("Error: Operation file corrupted.\n"));
	if (fscanf(f, "%d %d %c\n", &bg.w, &bg.h, &bg.c) != 3)
		return (clear_all(f, NULL) && str_error("Error: Operation file corrupted\n"));
	if (bg.w <= 0.00000000 || bg.w > 300.00000000 || bg.h <= 0.00000000 || bg.h > 300.00000000)
		return (clear_all(f, NULL) && str_error("Error: Operation file corrupted\n"));
	if (!(z = malloc(sizeof(char) * bg.w * bg.h)))
		return (clear_all(f, NULL) && str_error("Error: Malloc failed.\n"));
	i = 0;
	while (i < bg.h * bg.w)
		z[i++] = bg.c;
	while ((sr = fscanf(f, "%c %f %f %f %f %c\n", &r.t, &r.x, &r.y, &r.w, &r.h, &r.c)) == 6)
	{
		if (r.w <= 0.00000000 || r.h <= 0.00000000 || (r.t != 'r' && r.t != 'R'))
			return (clear_all(f, z) && str_error("Error: Operation file corrupted\n"));
		i = 0;
		while (i < bg.h)
		{
			j = 0;
			while (j < bg.w)
			{
				x = (float)j;
				y = (float)i;
				if (x < r.x || r.x + r.w < x || y < r.y || r.y + r.h < y)
					cr = 'O';
				else if (x - r.x < 1.00000000 || r.x + r.w - x < 1.00000000 || y - r.y < 1.00000000 || r.y + r.h - y < 1.00000000)
					cr = 'B';
				else
					cr = 'I';
				if (cr == 'B' || (cr == 'I' && r.t == 'R'))
					z[i * bg.w + j] = r.c;
				j++;
			}
			i++;
		}
	}
	if (sr != -1)
		return (clear_all(f, z) && str_error("Error: Operation file corrupted\n"));
	i = 0;
	while (i < bg.h)
	{
		write(1, z + (i * bg.w), bg.w);
		write(1, "\n", 1);
		i++;
	}
	clear_all(f, z);
	return (0);
}
