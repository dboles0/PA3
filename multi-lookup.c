#include "multi-lookup.h"
#include "util.h"
// open file in main and pass the pointer to producer 
// to start this process. it then will give it a thread/file name 
// it will read the file then proccess it to the queue
// this will complete then exit the main program


void * parser_thread(void *ptr){
	
	struct parser_info a_psr_info = *((struct parser_info *) ptr);
	FILE * fp;
	char * root = "input/";
	char * file_path;

	char str[MAX_NAME_LENGTH];                         // string read form file

	// save thread Id 
	a_psr_info.thread_id = pthread_self();

	//pthread_mutex_lock(&lock);
	// add root file to file to service
	file_path = concat(root, a_psr_info.file_name);	

	fp = fopen(file_path, "r");
	if(fp == NULL){ printf("error - opening file\n"); }
	while(!feof(fp)){	

		pthread_mutex_lock(&lock);


		while(q_is_full(a_psr_info.Q) == 1){
			pthread_cond_wait(&needs_less, &lock);
		}

		fgets(str, MAX_NAME_LENGTH, fp);
		enqueue(a_psr_info.Q, str);

		pthread_cond_signal(&needs_more);	
		pthread_mutex_unlock(&lock);
			
	}
	if(!feof(fp)){
		perror("EOF error");
	}	
	// close file
	fclose(fp);

	// free all information instide the struct
	free(file_path);
	free(a_psr_info.file_name);

	files_serviced += 1;
	if(files_serviced == a_psr_info.num_input_files){ done = true; } 
	// validate buffer has correct numbrer of elements
	pthread_exit(0);
}

void * converter_thread(void *ptr){

	struct converter_info a_conv_info = *((struct converter_info *) ptr);
	a_conv_info.thread_id = pthread_self();

	while(!done){
		while(a_conv_info.Q->q_size > 0){
			pthread_mutex_lock(&lock);
			while(q_is_empty(a_conv_info.Q) == 1){
				pthread_cond_wait(&needs_more, &lock);
			}
			dequeue(a_conv_info.Q);

			pthread_cond_signal(&needs_less);	
			pthread_mutex_unlock(&lock);
		}
	}
	// removes the domain name form the shared array
	//dnslookup(a_conv_info.Q->q_array[a_conv_info.Q->head], NULL, a_conv_info.Q->q_max);

	// query the IP Address
	// after name has been mapped to IP address write log
	// if queue is empty block until new item or all parser threads have terminated
	pthread_exit(0);
}


char * concat(char * str1, char * str2){
	char * ret = malloc(strlen(str1) + strlen(str2) + 1);
	strcpy(ret, str1);
	strcpy(ret+(strlen(str1)), str2);
	return ret;
}
