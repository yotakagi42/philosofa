/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:08:30 by yotakagi          #+#    #+#             */
/*   Updated: 2025/11/28 16:23:06 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static inline bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static inline bool	is_space(char c)
{
	return ((c >= 9 && c <= 13) || 32 == c);
}

static const char	*valid_input(const char *str, int *error)
{
	int			len;
	const char	*number;

	len = 0;
	while (is_space(*str))
		++str;
	if (*str == '+')
		++str;
	else if (*str == '-')
		*error = print_error("positiv number only");
	if (!is_digit(*str))
		*error = print_error("The input isnot correct");
	if (*error)
		return (NULL);
	number = str;
	while (is_digit(*str++))
		++len;
	if (len > 10)
		*error = print_error("The value is too big,INT_MAX is the limits");
	return (number);
}

static long	ft_atol(const char *str, int *error)
{
	long	num;

	num = 0;
	str = valid_input(str, error);
	if (*error)
		return (-1);
	while (is_digit(*str))
		num = (num * 10) + (*str++ - 48);
	if (num > INT_MAX)
		*error = print_error("The value is too big,INT_MAX is the limits");
	return (num);
}

int	parse_input(t_table *table, char **av)
{
	int	error;

	error = 0;
	table->philo_nbr = ft_atol(av[1], &error);
	table->time_to_die = ft_atol(av[2], &error) * 1e3;
	table->time_to_eat = ft_atol(av[3], &error) * 1e3;
	table->time_to_sleep = ft_atol(av[4], &error) * 1e3;
	if (av[5])
		table->nbr_limit_meals = ft_atol(av[5], &error);
	else
		table->nbr_limit_meals = -1;
	if (error)
		return (1);
	if (table->philo_nbr <= 0)
		return (print_error("Wrong input: number_of_philosophers must be > 0"));
	if (table->time_to_die < 6e4 || table->time_to_eat < 6e4
		|| table->time_to_sleep < 6e4)
		return (print_error("Use timetamps major than 60ms"));
	return (0);
}
