#include "multi-lookup.h"
#include "util.h"
// open file in main and pass the pointer to producer 
// to start this process. it then will give it a thread/file name 
// it will read the file then proccess it to the queue
// this will complete then exit the main program


void * parser_thread(void *ptr){
	
	struct parser_info a_psr_info = *((struct parser_info *) ptr);
	FILE * fp = a_psr_info.fp;
	int i;
	
	char str[MAX_NAME_LENGTH];                         // string read form file

	// save thread Id 
	a_psr_info.thread_id = pthread_self();

	// pthread_mutex_lock(&lock);
	// add root file to file to service

	if(fp == NULL){ printf("error - opening file\n"); }
	while(!feof(fp)){

		pthread_mutex_lock(&lock);
		while(q_is_full(a_psr_info.Q) == 1){
			pthread_cond_wait(&needs_less, &lock);
		}

		fgets(str, MAX_NAME_LENGTH, fp);
		i = 0;
		i = strlen(str);
		str[i-1] = '\0';
		enqueue(a_psr_info.Q, str);

		pthread_cond_signal(&needs_more);	
		pthread_mutex_unlock(&lock);

	}
	if(!feof(fp)){
		perror("EOF error");
	}	


	files_serviced += 1;
	if(files_serviced == a_psr_info.num_input_files){ done = true; } 

	pthread_mutex_lock(&psr_log_lock);
	fprintf(a_psr_info.psr_log_fp, "<%d> thread serviced %d files\n", a_psr_info.thread_id, a_psr_info.num_files_serviced);
	pthread_mutex_unlock(&psr_log_lock);

	// validate buffer has correct numbrer of elements
	pthread_exit(0);
}

void * converter_thread(void *ptr){

	FILE * fp;
	struct converter_info a_conv_info = *((struct converter_info *) ptr);
	char ipstr[MAX_IP_LENGTH];                                             // ip address
	char * url;
	a_conv_info.thread_id = pthread_self();

	
	while(!done){
		pthread_mutex_lock(&lock);
		fp = fopen("results-ref.txt", "w");
		while(a_conv_info.Q->q_size > 0){
			// if queue is empty block until new item or all parser threads have terminated
			while(q_is_empty(a_conv_info.Q) == 1){
				pthread_cond_wait(&needs_more, &lock);
			}

			url = a_conv_info.Q->q_array[a_conv_info.Q->head];
			// query the IP Address
			if(dnslookup(url, ipstr, sizeof(ipstr)) == UTIL_FAILURE){
				strncpy(ipstr, "", sizeof(ipstr));
			}
			printf("%s:%s\n", url, ipstr);
	
			fprintf(fp, "%s,%s\n", url, ipstr);
			// remove domain name form queue
			dequeue(a_conv_info.Q);

			pthread_cond_signal(&needs_less);	
			pthread_mutex_unlock(&lock);

		}
	}

	fclose(fp);

	// after name has been mapped to IP address write log
	pthread_exit(0);
}


char * concat(char * str1, char * str2){
	char * ret = malloc(strlen(str1) + strlen(str2) + 1);
	strcpy(ret, str1);
	strcpy(ret+(strlen(str1)), str2);
	return ret;
}

