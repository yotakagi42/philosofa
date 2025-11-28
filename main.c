/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:08:21 by yotakagi          #+#    #+#             */
/*   Updated: 2025/11/28 17:22:31 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	dinner_start(t_table *table)
{
	if (0 == table->nbr_limit_meals)
		return (0);
	if (create_threads(table) != 0)
		return (1);
	table->start_simulation = gettime(MILLISECOND);
	set_bool(&table->table_mutex, &table->all_threads_ready, true);
	join_threads(table);
	return (0);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 5 && ac != 6)
		return (print_error("Wrong input \n"));
	if (parse_input(&table, av) != 0)
		return (1);
	if (data_init(&table) != 0)
	{
		clean(&table);
		return (1);
	}
	if (dinner_start(&table) != 0)
	{
		clean(&table);
		return (1);
	}
	clean(&table);
	return (0);
}
