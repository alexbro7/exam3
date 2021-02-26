#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct	s_background
{
	int		w;
	int		h;
	char	c;
}				t_background;

typedef struct	s_circle
{
	char	t;
	float	x;
	float	y;
	float	d;
	char	c;
}				t_circle;

int		clear_all(FILE *f, char *z)
{
	fclose(f);
	if (z)
		free(z);
	return (1);
}

int		str_error(char *s)
{
	int	i = 0;
	while (s[i])
		i++;
	write(1, s, i);
	return (1);
}

int		main(int ac ,char **av)
{
	t_background	bg;
	t_circle		c;
	FILE			*f;
	char			*z, rr;
	int				sr, i, j;
	float			x, y, d;

	if (ac != 2)
		return (str_error("Error: argument\n"));
	if (!(f = fopen(av[1], "r")))
		return (str_error("Error: Operation file corrupted\n"));
	if (fscanf(f, "%d %d %c\n", &bg.w, &bg.h, &bg.c) != 3)
		return (clear_all(f, NULL) &&str_error("Error: Operation file corrupted\n"));
	if (bg.w <= 0.00000000 || bg.w > 300.00000000 || bg.h <= 0.00000000 || bg.h > 300.00000000)
		return (clear_all(f, NULL) &&str_error("Error: Operation file corrupted\n"));
	if (!(z = malloc(sizeof(char) * bg.w * bg.h)))
		return (clear_all(f, NULL) &&str_error("Error: Malloc failed\n"));
	i = 0;
	while (i < bg.w * bg.h)
		z[i++] = bg.c;
	while ((sr = fscanf(f, "%c %f %f %f %c\n", &c.t, &c.x, &c.y, &c.d, &c.c)) == 5)
	{
		if (c.d <= 0.00000000 || (c.t != 'c' && c.t != 'C'))
			return (clear_all(f, z) &&str_error("Error: Operation file corrupted\n"));
		i = 0;
		while (i < bg.h)
		{
			j = 0;
			while (j < bg.w)
			{
				x = (float)j;
				y = (float)i;
				d = sqrtf((c.x - x) * (c.x - x) + (c.y - y) * (c.y - y));
				if (d > c.d)
					rr = 'O';
				else if (c.d - d < 1.00000000)
					rr = 'B';
				else
					rr = 'I';
				if (rr == 'B' || (rr == 'I' && c.t == 'C'))
					z[i * bg.w + j] = c.c;
				j++;
			}
			i++;
		}
	}
	if (sr != -1)
		return (clear_all(f, z) &&str_error("Error: Operation file corrupted\n"));
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
