/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 17:54:37 by omoussao          #+#    #+#             */
/*   Updated: 2021/11/03 19:00:43 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_size(long long nbr)
{
	int	size;

	size = (nbr < 0) + !(nbr);
	while (nbr)
	{
		nbr /= 10;
		size++;
	}
	return (size + 1);
}

static int	power(long long base, long long exp)
{
	if (exp <= 0)
		return (exp == 0);
	if (exp % 2 == 0)
		return (power(base * base, exp / 2));
	else
		return (base * power(base * base, (exp - 1) / 2));
}

char	*ft_itoa(int n)
{
	char		*str;
	int			size;
	int			int_power;
	int			i;
	long long	nbr;

	nbr = (long long)n;
	size = get_size(nbr);
	int_power = power(10, size - 2 - (nbr < 0));
	str = malloc(size);
	if (!str)
		return (NULL);
	str[size - 1] = 0;
	i = (nbr < 0);
	if (nbr < 0)
	{
		str[0] = '-';
		nbr *= -1;
	}
	while (int_power)
	{
		str[i++] = (nbr / int_power) % 10 + '0';
		int_power /= 10;
	}
	return (str);
}
