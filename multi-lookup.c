#include "multi-lookup.h"
// open file in main and pass the pointer to producer 
// to start this process. it then will give it a thread/file name 
// it will read the file then proccess it to the queue
// this will complete then exit the main program

void * parser_thread(void *param){
	// repeadedly read line form given param(given file)
	// add entry into shared buffer
	// validate buffer has correct numbrer of elements
	pthread_exit(0);
}

void * converter_thread(void *param){
	// removes the domain name form the shared array
	// query the IP Address
	// after name has been mapped to IP address write log
	// if queue is empty block until new item or all parser threads have terminated
	pthread_exit(0);
}

