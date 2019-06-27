#include "fifo_queue.h"


struct request_info{
	int thread_name;
	FILE * fp;  
	struct fifo_Q * queue;	
};

void * runner(void *param);  
//void get_file_names(char * update_file_names[], int * a_num_files, int * a_buffer_size);
