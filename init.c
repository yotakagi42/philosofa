/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:08:10 by yotakagi          #+#    #+#             */
/*   Updated: 2025/11/28 16:23:10 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int philo_position)
{
	int	philo_nbr;

	philo_nbr = philo->table->philo_nbr;
	philo->first_fork = &forks[philo_position];
	philo->second_fork = &forks[(philo_position + 1) % philo_nbr];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[(philo_position + 1) % philo_nbr];
		philo->second_fork = &forks[philo_position];
	}
}

static int	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->full = false;
		philo->meals_counter = 0;
		philo->table = table;
		if (safe_mutex_handle(&philo->philo_mutex, INIT) != 0)
			return (1);
		assign_forks(philo, table->forks, i);
	}
	return (0);
}

int	data_init(t_table *table)
{
	int	i;

	i = -1;
	table->end_simulation = false;
	table->all_threads_ready = false;
	table->threads_running_nbr = 0;
	table->philos = safe_malloc(sizeof(t_philo) * table->philo_nbr);
	if (!table->philos)
		return (1);
	table->forks = safe_malloc(sizeof(t_fork) * table->philo_nbr);
	if (!table->forks)
	{
		free(table->philos);
		return (1);
	}
	if (safe_mutex_handle(&table->table_mutex, INIT) != 0
		|| safe_mutex_handle(&table->write_mutex, INIT) != 0)
		return (1);
	while (++i < table->philo_nbr)
		if (safe_mutex_handle(&table->forks[i].fork, INIT) != 0)
			return (1);
	return (philo_init(table));
}
