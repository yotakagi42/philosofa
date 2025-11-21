/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:09:24 by yotakagi          #+#    #+#             */
/*   Updated: 2025/11/21 16:43:42 by yotakagi         ###   ########.fr       */
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

#define RST "\033[0m"
#define RED "\033[1;31m"
#define G "\033[1;32m"
#define Y "\033[1;33m"
#define B "\033[1;34m"
#define A "\033[1;35m"
#define C "\033[1;36m"
#define W "\033[1;37m"

#define DEBUG_MODE 1

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
	int					fork_id;
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

void					error_exit(const char *error);
void					parse_input(t_table *table, char **av);
void					*safe_malloc(size_t bytes);
void					safe_thread_handle(pthread_t *thread,
							void *(*foo)(void *), void *data, t_opcode opcode);
void					safe_mutex_handle(t_mtx *mutex, t_opcode opcode);
void					data_init(t_table *table);
void					set_bool(t_mtx *mutex, bool *dest, bool value);
bool					get_bool(t_mtx *mutex, bool *value);
void					set_long(t_mtx *mutex, long *dest, long value);
long					get_long(t_mtx *mutex, long *value);
bool					simulation_finished(t_table *table);
void					wait_all_threads(t_table *table);
long					gettime(t_time_code time_code);
void					precise_usleep(long usec, t_table *table);
void					write_status(t_philo_status status, t_philo *philo,
							bool debug);
void					parse_input(t_table *table, char **av);
void					dinner_start(t_table *table);
bool					all_threads_running(t_mtx *mutex, long *threads,
							long philo_nbr);
void					increase_long(t_mtx *mutex, long *value);
void					*monitor_dinner(void *data);
void					clean(t_table *table);