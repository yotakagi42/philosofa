/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:09:24 by yotakagi          #+#    #+#             */
/*   Updated: 2025/11/28 17:42:25 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}						t_philo_status;

typedef enum e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}						t_opcode;

typedef enum e_get_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND,
}						t_time_code;

typedef pthread_mutex_t	t_mtx;
typedef struct s_table	t_table;

/*fork*/

typedef struct s_fork
{
	t_mtx				fork;
}						t_fork;

/*philo
./philo 5 800 200 200 [5]
*/

typedef struct s_philo
{
	int					id;
	long				meals_counter;
	bool				full;
	long				last_meal_time;
	t_fork				*first_fork;
	t_fork				*second_fork;
	pthread_t			thread_id;
	t_mtx				philo_mutex;
	t_table				*table;
}						t_philo;

/*table
./philo 5 800 200 200 [5]
*/
typedef struct s_table
{
	long				philo_nbr;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				nbr_limit_meals;
	long				start_simulation;
	bool				end_simulation;
	bool				all_threads_ready;
	long				threads_running_nbr;
	pthread_t			monitor;
	t_mtx				table_mutex;
	t_mtx				write_mutex;
	t_fork				*forks;
	t_philo				*philos;
}						t_table;

int						print_error(const char *error);
int						parse_input(t_table *table, char **av);
void					*safe_malloc(size_t bytes);
int						safe_thread_handle(pthread_t *thread,
							void *(*foo)(void *), void *data, t_opcode opcode);
int						safe_mutex_handle(t_mtx *mutex, t_opcode opcode);
int						data_init(t_table *table);
void					set_bool(t_mtx *mutex, bool *dest, bool value);
bool					get_bool(t_mtx *mutex, bool *value);
void					set_long(t_mtx *mutex, long *dest, long value);
long					get_long(t_mtx *mutex, long *value);
bool					simulation_finished(t_table *table);
void					wait_all_threads(t_table *table);
long					gettime(t_time_code time_code);
void					precise_usleep(long usec, t_table *table);
void					write_status(t_philo_status status, t_philo *philo);
int						dinner_start(t_table *table);
bool					all_threads_running(t_mtx *mutex, long *threads,
							long philo_nbr);
void					increase_long(t_mtx *mutex, long *value);
void					*monitor_dinner(void *data);
void					clean(t_table *table);
void					*lone_philo(void *arg);
void					join_threads(t_table *table);
int						create_threads(t_table *table);
