/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldufour <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 08:07:08 by ldufour           #+#    #+#             */
/*   Updated: 2024/01/18 10:12:21 by ldufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
 * Inspiration from Oceano
 */

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (!ret)
		exit_prg_at_error("Malloc failure");
	return (ret);
}

void	exit_prg_at_error(char *str)
{
	printf("Error\n");
	if (str)
		printf("%s\n", str);
	exit(EXIT_FAILURE);
}

static void	handle_mutex_error(int status, t_opcode opcode)
{
	if (0 == status)
		return ;
	if (EINVAL == status && (LOCK == opcode || UNLOCK == opcode))
		exit_prg_at_error("The value specified by mutex is invalid");
	else if (EINVAL == status && INIT == opcode)
		exit_prg_at_error("The value specified by attr is invalid.");
	else if (EDEADLK == status)
		exit_prg_at_error("A deadlock would occur if the thread "
			"blocked waiting for mutex.");
	else if (EPERM == status)
		exit_prg_at_error("The current thread does not hold a lock on mutex.");
	else if (ENOMEM == status)
		exit_prg_at_error("The process cannot allocate enough memory"
			" to create another mutex.");
	else if (EBUSY == status)
		exit_prg_at_error("Mutex is locked");
}

/*
 * THREADS errors
*/
static void	handle_thread_error(int status, t_opcode opcode)
{
	if (0 == status)
		return ;
	if (EAGAIN == status)
		exit_prg_at_error("No resources to create another thread");
	else if (EPERM == status)
		exit_prg_at_error("The caller does not have appropriate permission\n");
	else if (EINVAL == status && CREATE == opcode)
		exit_prg_at_error("The value specified by attr is invalid.");
	else if (EINVAL == status && (JOIN == opcode || DETACH == opcode))
		exit_prg_at_error("The value specified by thread is not joinable\n");
	else if (ESRCH == status)
		exit_prg_at_error("No thread could be found corresponding to that"
			"specified by the given thread ID, thread.");
	else if (EDEADLK == status)
		exit_prg_at_error("A deadlock was detected or the value of"
			"thread specifies the calling thread.");
}

/*
 * One function to handle safely
 * lock unlock init destroy
*/
void	safe_mutex_handle(pthread_mutex_t *mutex, t_opcode opcode)
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
		exit_prg_at_error("Wrong opcode for mutex_handle:"
			"use <LOCK> <UNLOCK> <INIT> <DESTROY>");
}

/*
 * One function to handle threads
 * create join detach
*/
void	safe_thread_handle(pthread_t *thread, void *(*foo)(void *),
		void *data, t_opcode opcode)
{
	if (CREATE == opcode)
		handle_thread_error(pthread_create(thread, NULL, foo, data), opcode);
	else if (JOIN == opcode)
		handle_thread_error(pthread_join(*thread, NULL), opcode);
	else if (DETACH == opcode)
		handle_thread_error(pthread_detach(*thread), opcode);
	else
		exit_prg_at_error("Wrong opcode for thread_handle:"
			" use <CREATE> <JOIN> <DETACH>");
}

