/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:07:59 by yotakagi          #+#    #+#             */
/*   Updated: 2025/11/28 17:08:57 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	thinking(t_philo *philo)
{
	if (philo->table->philo_nbr % 2 != 0)
		usleep(1000);
}

static void	eat(t_philo *philo)
{
	safe_mutex_handle(&philo->first_fork->fork, LOCK);
	safe_mutex_handle(&philo->second_fork->fork, LOCK);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILLISECOND));
	philo->meals_counter++;
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->nbr_limit_meals > 0
		&& philo->meals_counter == philo->table->nbr_limit_meals)
		set_bool(&philo->philo_mutex, &philo->full, true);
	safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
	safe_mutex_handle(&philo->second_fork->fork, UNLOCK);
}

void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->threads_running_nbr);
	if (philo->id % 2 == 0)
		usleep(15000);
	while (!simulation_finished(philo->table))
	{
		if (philo->full)
			break ;
		eat(philo);
		precise_usleep(philo->table->time_to_sleep, philo->table);
		thinking(philo);
	}
	return (NULL);
}

static int	create_threads(t_table *table)
{
	int	i;

	i = -1;
	if (table->philo_nbr == 1)
	{
		if (safe_thread_handle(&table->philos[0].thread_id, lone_philo,
				&table->philos[0], CREATE) != 0)
			return (1);
	}
	else
	{
		while (++i < table->philo_nbr)
			if (safe_thread_handle(&table->philos[i].thread_id,
					dinner_simulation, &table->philos[i], CREATE) != 0)
				return (1);
	}
	if (safe_thread_handle(&table->monitor, monitor_dinner, table, CREATE) != 0)
		return (1);
	return (0);
}

static void	join_threads(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_nbr)
		safe_thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&table->table_mutex, &table->end_simulation, true);
	safe_thread_handle(&table->monitor, NULL, NULL, JOIN);
}
