/*
 * File: multi-lookup.h
 * Author: Donald Boles
 * Project: CSCI 3753 Programming Assignment 3
 * Description:
 * 	functionallity of threads
 */

#ifndef MULTI_LOOKUP_H
#define MULTI_LOOKUP_H

#include "test.h"
#include "fifo_queue.h"

extern pthread_mutex_t lock;
extern pthread_cond_t needs_less;
extern pthread_cond_t needs_more;
extern bool done; 

struct parser_info{
	int thread_id;
	int num_files_serviced;
	struct fifo_Q * Q;		
	char * file_name;

};

struct converter_info{
	int thread_id;
	struct fifo_Q * Q;		

};

void * parser_thread(void *ptr);
void * converter_thread(void *param);
char * concat(char * str1, char * str2);

#endif
