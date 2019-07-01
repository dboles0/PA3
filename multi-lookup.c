#include "multi-lookup.h"
// open file in main and pass the pointer to producer 
// to start this process. it then will give it a thread/file name 
// it will read the file then proccess it to the queue
// this will complete then exit the main program

void * parser_thread(void *ptr){
	
	struct thread_info t_data = *((struct thread_info *) ptr);
	FILE * fp;
	char * file_path = "input/";

	printf("%s\n", t_data.file_name);
	struct fifo_Q * q = t_data.Q;
	char str[MAX_NAME_LENGTH];                                   // string read form file

	// add file location to param


	concat(file_path, t_data.file_name);	
	printf("file path = %s\n", t_data.file_name);
	printf("queue = %s\n", t_data.Q->q_array[0]);
	// repeadedly read line form given param(given file)
	

	fp = fopen(file_path, "r");
	while(!feof(fp)){	
		fgets(str, MAX_NAME_LENGTH, fp);
		enqueue(q, str);
		queue_state(q);
	}
	if(!feof(fp)){
		perror("EOF error");
	}

	// close file
	fclose(fp);

	//free(t_info);
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

struct thread_info * create_thread_info(void){
	struct thread_info * t_info = malloc(sizeof(struct thread_info));
	t_info->Q = NULL;
	t_info->file_name = malloc(5);
	t_info->file_name = "info";
	return t_info;	
}

char * concat(char * str1, char * str2){
	char * ret = malloc(strlen(str1) + strlen(str2) + 1);
	strcpy(ret, str1);
	strcpy(ret+(strlen(str1)), str2);
	return ret;
}
