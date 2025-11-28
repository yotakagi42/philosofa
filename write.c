/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:09:03 by yotakagi          #+#    #+#             */
/*   Updated: 2025/11/28 16:47:56 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static const char	*status_to_string(t_philo_status status)
{
	if (EATING == status)
		return ("is eating");
	if (SLEEPING == status)
		return ("is sleeping");
	if (THINKING == status)
		return ("is thinking");
	if (TAKE_FIRST_FORK == status || TAKE_SECOND_FORK == status)
		return ("has taken a fork");
	if (DIED == status)
		return ("died");
	return ("");
}

void	write_status(t_philo_status status, t_philo *philo)
{
	long	elapsed;

	elapsed = gettime(MILLISECOND) - philo->table->start_simulation;
	if (get_bool(&philo->philo_mutex, &philo->full))
		return ;
	safe_mutex_handle(&philo->table->write_mutex, LOCK);
	if (!simulation_finished(philo->table))
	{
		printf("%-10ld %d %s\n", elapsed, philo->id,
			status_to_string(status));
	}
	else if (DIED == status)
	{
		printf("%-10ld %d %s\n", elapsed, philo->id, status_to_string(status));
	}
	safe_mutex_handle(&philo->table->write_mutex, UNLOCK);
}
