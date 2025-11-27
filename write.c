/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:09:03 by yotakagi          #+#    #+#             */
/*   Updated: 2025/11/27 16:21:32 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static const char	*status_to_string(t_philo_status status)
{
	if (status == EATING)
		return ("EATING");
	if (status == SLEEPING)
		return ("SLEEPING");
	if (status == THINKING)
		return ("THINKING");
	if (status == TAKE_FIRST_FORK)
		return ("TAKE_FIRST_FORK");
	if (status == TAKE_SECOND_FORK)
		return ("TAKE_SECOND_FORK");
	if (status == DIED)
		return ("DIED");
	return ("UNKNOWN");
}

static void	write_status_debug(t_philo_status status, t_philo *philo,
		long elapsed)
{
	printf(C "[DEBUG] %-6ldms | Ph#%d | %-18s | Meals: %ld | Full: %d\n" RST,
		elapsed, philo->id, status_to_string(status), philo->meals_counter,
		philo->full);
}

void	write_status(t_philo_status status, t_philo *philo, bool debug)
{
	long	elapsed;

	elapsed = gettime(MILLISECOND) - philo->table->start_simulation;
	if (philo->full)
		return ;
	safe_mutex_handle(&philo->table->write_mutex, LOCK);
	if (debug)
		write_status_debug(status, philo, elapsed);
	else
	{
		if ((TAKE_FIRST_FORK == status || TAKE_SECOND_FORK == status)
			&& !simulation_finished(philo->table))
			printf("%ld %d has taken a fork\n", elapsed, philo->id);
		else if (EATING == status && !simulation_finished(philo->table))
			printf("%ld %d is eating\n", elapsed, philo->id);
		else if (SLEEPING == status && !simulation_finished(philo->table))
			printf("%ld %d is sleeping\n", elapsed, philo->id);
		else if (THINKING == status && !simulation_finished(philo->table))
			printf("%ld %d is thinking\n", elapsed, philo->id);
		else if (DIED == status)
			printf("%ld %d died\n", elapsed, philo->id);
	}
	safe_mutex_handle(&philo->table->write_mutex, UNLOCK);
}
