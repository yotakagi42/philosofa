/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:08:41 by yotakagi          #+#    #+#             */
/*   Updated: 2025/11/28 17:44:30 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (NULL == ret)
	{
		print_error("Error with the malloc");
		return (NULL);
	}
	return (ret);
}

static int	handle_mutex_error(int status, t_opcode opcode)
{
	if (0 == status)
		return (0);
	if (EINVAL == status && (LOCK == opcode || UNLOCK == opcode))
		return (print_error("Invalid mutex value"));
	else if (EINVAL == status && INIT == opcode)
		return (print_error("The value specified by attr is invalid"));
	else if (EDEADLK == status)
		return (print_error("Mutex deadlock detected"));
	else if (EPERM == status)
		return (print_error("Mutex permission error"));
	else if (ENOMEM == status)
		return (print_error("Mutex memory allocation failed"));
	else if (EBUSY == status)
		return (print_error("Mutex is locked"));
	return (print_error("Unknown mutex error"));
}

int	safe_mutex_handle(t_mtx *mutex, t_opcode opcode)
{
	if (LOCK == opcode)
		return (handle_mutex_error(pthread_mutex_lock(mutex), opcode));
	else if (UNLOCK == opcode)
		return (handle_mutex_error(pthread_mutex_unlock(mutex), opcode));
	else if (INIT == opcode)
		return (handle_mutex_error(pthread_mutex_init(mutex, NULL), opcode));
	else if (DESTROY == opcode)
		return (handle_mutex_error(pthread_mutex_destroy(mutex), opcode));
	else
		return (print_error("Wrong opcode for mutex handle"));
}

static int	handle_thread_error(int status, t_opcode opcode)
{
	if (0 == status)
		return (0);
	if (EAGAIN == status)
		return (print_error("Thread creation resource error"));
	else if (EPERM == status)
		return (print_error("Thread permission error"));
	else if (EINVAL == status && CREATE == opcode)
		return (print_error("The value specified by attr is invalid"));
	else if (EINVAL == status && (JOIN == opcode || DETACH == opcode))
		return (print_error("The thread is not a joinable thread"));
	else if (ESRCH == status)
		return (print_error("Thread ID not found"));
	else if (EDEADLK == status)
		return (print_error("Thread deadlock detected"));
	return (print_error("Unknown thread error"));
}

int	safe_thread_handle(pthread_t *thread, void *(*foo)(void *), void *data,
		t_opcode opcode)
{
	if (CREATE == opcode)
		return (handle_thread_error(pthread_create(thread, NULL, foo, data),
				opcode));
	else if (JOIN == opcode)
		return (handle_thread_error(pthread_join(*thread, NULL), opcode));
	else if (DETACH == opcode)
		return (handle_thread_error(pthread_detach(*thread), opcode));
	else
		return (print_error("Wrong opcode for thread handle"));
}
