/*
 * File: fifo_queue.h
 * Author: Donald Boles
 * Project: CSCI 3753 Programming Assignment 3
 * Description:
 * 	This class stores input in a queue class	
 */

#ifndef FIFO_QUEUE_H
#define FIFO_QUEUE_H

/* Define the following to enable debug statments */
// #define UTIL_DEBUG

#include <stdlib.h>
#include <pthread.h>                   // therads
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>                     // handeling file error check
#include <unistd.h>                    // usleep, 
#include <stdbool.h>                   

#define MIN_ARGS 1
#define MAX_INPUT_FILES 10
#define MAX_CONVERT_THREADS 10
#define MAX_NAME_LENGTH 1025
#define MAX_IP_LENGTH INET6_ADDRSTRLEN


struct fifo_Q{
	int head, tail, q_size, q_max;
	char ** q_array;
};

struct fifo_Q * create_queue(unsigned q_max);
int enqueue(struct fifo_Q * a_queue, char * entry);
void dequeue(struct fifo_Q * a_queue);
int q_is_empty(struct fifo_Q * a_queue);
int q_is_full(struct fifo_Q * a_queue);
char * copy(char * str);
void clear_element(char * str);


#endif
