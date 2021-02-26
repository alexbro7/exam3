#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
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
	int		i;

	i = 0;
	while(s[i])
		i++;
	write(1, s, i);
	return (1);
}

int		get_background(t_background *bg, FILE *f)
{
	if (fscanf(f, "%d %d %c\n", &bg->w, &bg->h, &bg->c) != 3)
		return (0);
	if (bg->w <= 0.00000000 || bg->w > 300.00000000 || bg->h <= 0.00000000 || bg->h > 300.00000000)
		return (0);
	return (1);
}

int		in_circle(float x, float y, t_circle c)
{
	float	d;

	d = sqrtf((c.x - x) * (c.x - x) + (c.y - y) * (c.y - y));
	if (c.d < d)
		return ('0');
	if (c.d - d < 1.00000000)
		return ('2');
	return ('1');
}

void	draw_circle(char **z, t_circle c, t_background bg)
{
	int		i;
	int		j;
	int		r;

	i = 0;
	while (i < bg.h)
	{
		j = 0;
		while (j < bg.w)
		{
			r = in_circle((float)j, (float)i, c);
			if (r == '2' || (r == '1' && c.t == 'C'))
				(*z)[(i * bg.w) + j] = c.c;
			j++;
		}
		i++;
	}
}

int		make_rendering(char **z, t_background bg, FILE *f)
{
	t_circle	c;
	int			sr;
	int			i;

	i = 0;
	while (i < bg.w * bg.h)
		(*z)[i++] = bg.c;
	while ((sr = fscanf(f, "%c %f %f %f %c\n", &c.t, &c.x, &c.y, &c.d, &c.c)) == 5)
	{
///**/printf("%c %f %f %f %c\n", c.t, c.x, c.y, c.d, c.c);
		if (c.d <= 0.00000000 || (c.t != 'c' && c.t != 'C'))
			return (0);
		draw_circle(z, c, bg);
	}
	return (sr == -1);
}

void	print_rendering(char *z, t_background bg)
{
	int		i;

	i = 0;
	while (i < bg.h)
	{
		write(1, z + (bg.w * i), bg.w);
		write(1, "\n", 1);
		i++;
	}
}

int		main(int ac, char **av)
{
	t_background	bg;
	FILE			*f;
	char			*z;

	if (ac != 2)
		return (str_error("Error: argument\n"));
	if (!(f = fopen(av[1], "r")))
		return (str_error("Error: Operation file corrupted\n"));
	if (!get_background(&bg, f))
		return (clear_all(f, NULL) && str_error("Error: Operation file corrupted\n"));
///**/printf("%d %d %c\n", bg.w, bg.h, bg.c);
	if (!(z = malloc(sizeof(char) * bg.w * bg.h)))
		return (clear_all(f, NULL) && str_error("Malloc failed\n"));
	if (!make_rendering(&z, bg, f))
		return (clear_all(f, z) &&str_error("Error: Operation file corrupted\n"));
	print_rendering(z, bg);
	clear_all(f, z);
	return (0);
}
