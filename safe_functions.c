/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:08:41 by yotakagi          #+#    #+#             */
/*   Updated: 2025/11/21 15:13:33 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (NULL == ret)
		error_exit("Error with the malloc");
	return (ret);
}

static void	handle_mutex_error(int status, t_opcode opcode)
{
	if (0 == status)
		return ;
	if (EINVAL == status && (LOCK == opcode || UNLOCK == opcode))
		error_exit("Invalid mutex value");
	else if (EINVAL == status && INIT == opcode)
		error_exit("The value specified by attr is invalid");
	else if (EDEADLK == status)
		error_exit("Mutex deadlock detected");
	else if (EPERM == status)
		error_exit("Mutex permission error");
	else if (ENOMEM == status)
		error_exit("Mutex memory allocation failed");
	else if (EBUSY == status)
		error_exit("Mutex is locked");
}

void	safe_mutex_handle(t_mtx *mutex, t_opcode opcode)
{
	if (LOCK == opcode)
		handle_mutex_error(pthread_mutex_lock(mutex), opcode);
	else if (UNLOCK == opcode)
		handle_mutex_error(pthread_mutex_unlock(mutex), opcode);
	else if (INIT == opcode)
		handle_mutex_error(pthread_mutex_init(mutex, NULL), opcode);
	else if (DESTROY == opcode)
		handle_mutex_error(pthread_mutex_destroy(mutex), opcode);
	else
		error_exit("Wrong opcode for mutex handle");
}

static void	handle_thread_error(int status, t_opcode opcode)
{
	if (0 == status)
		return ;
	if (EAGAIN == status)
		error_exit("Thread creation resource error");
	else if (EPERM == status)
		error_exit("Thread permission error");
	else if (EINVAL == status && CREATE == opcode)
		error_exit("The value specified by attr is invalid");
	else if (EINVAL == status && (JOIN == opcode || DETACH == opcode))
		error_exit("The thread is not a joinable thread");
	else if (ESRCH == status)
		error_exit("Thread ID not found");
	else if (EDEADLK == status)
		error_exit("Thread deadlock detected");
}

void	safe_thread_handle(pthread_t *thread, void *(*foo)(void *), void *data,
		t_opcode opcode)
{
	if (CREATE == opcode)
		handle_thread_error(pthread_create(thread, NULL, foo, data), opcode);
	else if (JOIN == opcode)
		handle_thread_error(pthread_join(*thread, NULL), opcode);
	else if (DETACH == opcode)
		handle_thread_error(pthread_detach(*thread), opcode);
	else
		error_exit("Wrong opcode for thread handle");
}
