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

struct thread_info{
	struct fifo_Q * Q;		
	char * file_name;

};

void * parser_thread(void *ptr);
void * converter_thread(void *param);
struct thread_info * create_thread_info(void);
char * concat(char * str1, char * str2);

#endif
