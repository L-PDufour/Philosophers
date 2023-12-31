/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldufour <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 08:30:18 by ldufour           #+#    #+#             */
/*   Updated: 2023/11/20 15:31:41 by ldufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

t_prg	*init_struct(void)
{
	static t_prg	*prg;

	prg = NULL;
	if (!prg)
	{
		prg = (t_prg *)malloc(sizeof(t_prg));
		if (!prg)
		{
			printf("Malloc failure\n");
			exit(EXIT_FAILURE);
		}
	}
	return (prg);
}

long long	timeInMilliseconds(void)
{
	static struct timeval	start = {0, 0};
	struct timeval			end;

	if (start.tv_sec == 0 && start.tv_usec == 0)
		gettimeofday(&start, NULL);
	gettimeofday(&end, NULL);
	return (((long long)end.tv_sec) * 1000 + (end.tv_usec / 1000))
		- (((long long)start.tv_sec) * 1000 + (start.tv_usec / 1000));
}

void	mutex_destroy(t_prg *prg)
{
	int	i;

	i = 0;
	while (i < prg->nb_of_philo)
	{
		pthread_mutex_destroy(&prg->philosophers->r_fork);
		i++;
	}
}

void	mutex_init(t_prg *prg)
{
	int	i;

	i = 0;
	while (i < prg->nb_of_philo)
	{
		pthread_mutex_init(&prg->philosophers->r_fork, NULL);
		i++;
	}
}

// void *action(void *data)
// {
//  t_cowboy cowboy;
//
//  cowboy = *(t_cowboy *)data;

void	thread_init(t_prg *prg, int argc, char **argv)
{
	int	i;

	i = 0;
	prg = init_struct();
	parsing_arguments(prg, argc, argv);
	prg->philosophers->l_fork = safe_malloc(sizeof(pthread_mutex_t));
	prg->threads = safe_malloc(sizeof(pthread_t) * prg->nb_of_philo);
	mutex_init(prg);
}

/*
*	void philo_is_eating(t_prg *prg)
*	{
*		if (prg->philo[i])
*			if (prg->fork[i] == unlock && prg->fork[i + 1] == unlock)
*				prg->fork[i] == lock
*				prg->fork[i + 1] == lock
*				printf (i is eating)
*				usleep(time_to_eat)
*				prg->fork[i] == unlock
*				prg->fork[i + 1] == unlock
*			philo[i + 1] must sleep time_to_sleep
*			if fork lock than think and wait
*			check if philo is not dead (time_before_last_meal)
*			if (nbr_of_meals) check if nbr of meals == 0 (nbr_of_meals--)
*			check if philo can eat (fork lock) else think and wait
*			if eated then check for sleep


}
*
*/

void	*dispatch(void *arg)
{
	t_prg	prg;

	prg = *(t_prg *)arg;
	return (NULL);
}

void	thread_create(t_prg *prg)
{
	int	i;

	i = 0;
	while (i < prg->nb_of_philo)
	{
		pthread_create(&prg->threads[i], NULL, dispatch, NULL);
		i++;
	}
}
void	thread_join(t_prg *prg)
{
	int	i;

	i = 0;
	while (i < prg->nb_of_philo)
	{
		pthread_join(prg->threads[i], NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_prg	*prg;
	int		i;

	i = 0;
	prg = NULL;
	if (argc == 5 || argc == 6)
	{
		thread_init(prg, argc, argv);
		// printf("%i\n", prg->nb_of_philo);
		// while (1)
		// 	printf("timepstamp in %lli\n", timeInMilliseconds());
		// pthread_mutex_destroy(&mutex);
		// printf("Total drinks served: %d\\n", g_drinks_served);
		thread_join(prg);
		mutex_destroy(prg);
	}
	else
		exit_prg_at_error("Invalid arguments");
	return (0);
}
