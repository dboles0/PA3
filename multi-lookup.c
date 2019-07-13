#include "multi-lookup.h"
#include "util.h"
// open file in main and pass the pointer to producer 
// to start this process. it then will give it a thread/file name 
// it will read the file then proccess it to the queue
// this will complete then exit the main program


void * parser_thread(void *ptr){
	
	struct parser_info a_psr_info = *((struct parser_info *) ptr);
	FILE * in_file_fp = a_psr_info.in_files_fp;
	FILE * log_fp = a_psr_info.psr_log_fp;
	int i;
		
	char str[MAX_NAME_LENGTH];                         // string read form file

	// save thread Id 
	a_psr_info.thread_id = pthread_self();

	pthread_mutex_lock(&q_lock);
	
	if(in_file_fp == NULL){ printf("error - opening file\n"); }
	while(!feof(in_file_fp)){	


		while(q_is_full(a_psr_info.Q) == 1){
			pthread_cond_wait(&needs_less, &q_lock);
		}

		fgets(str, MAX_NAME_LENGTH, in_file_fp);
		i = 0;
		i = strlen(str);
		str[i-1] = '\0';
		enqueue(a_psr_info.Q, str);

		pthread_cond_signal(&needs_more);	
		pthread_mutex_unlock(&q_lock);
	
			
	}
	if(!feof(in_file_fp)){
		perror("EOF error");
	}	
	
	// log parser thread info
	pthread_mutex_lock(&psr_log_lock);
	fprintf(log_fp, "<%i> thread serviced %i files\n", a_psr_info.thread_id, a_psr_info.num_files_serviced);
	pthread_mutex_unlock(&psr_log_lock);


	files_serviced += 1;
	if(files_serviced == a_psr_info.num_input_files){ done = true; } 
	// validate buffer has correct numbrer of elements

	
	pthread_exit(0);
}

void * converter_thread(void *ptr){

	struct converter_info a_conv_info = *((struct converter_info *) ptr);
	FILE * conv_log_fp = a_conv_info.conv_log_fp;
	char ipstr[MAX_IP_LENGTH] = {"000000000"};                                             // ip address


	a_conv_info.thread_id = pthread_self();
	
	while(!done){
		
		pthread_mutex_lock(&q_lock);
		while(a_conv_info.Q->q_size > 0){

			// if queue is empty block until new item or all parser threads have terminated
			while(q_is_empty(a_conv_info.Q) == 1){
				pthread_cond_wait(&needs_more, &q_lock);
			}

			// query the IP Address
			if(dnslookup(a_conv_info.Q->q_array[a_conv_info.Q->head], ipstr, sizeof(ipstr)) == UTIL_FAILURE){
				strncpy(ipstr, "", sizeof(ipstr));
			}
			printf("%s:%s\n", a_conv_info.Q->q_array[a_conv_info.Q->head], ipstr);

			// log converter thread info	
			pthread_mutex_lock(&conv_log_lock);
			fprintf(conv_log_fp, "%s, %s\n", a_conv_info.Q->q_array[a_conv_info.Q->head], ipstr);
			pthread_mutex_unlock(&conv_log_lock);

			// remove domain name form queue
			dequeue(a_conv_info.Q);

			pthread_cond_signal(&needs_less);	
			pthread_mutex_unlock(&q_lock);
		}
	}


	// after name has been mapped to IP address write log
	pthread_exit(0);
}


char * concat(char * str1, char * str2){
	char * ret = malloc(strlen(str1) + strlen(str2) + 1);
	strcpy(ret, str1);
	strcpy(ret+(strlen(str1)), str2);
	return ret;
}

char * conv_copy(char * str){
	int str_len = strlen(str);
	char * ptr = (char *) malloc (str_len + 1 * sizeof(char));
	memcpy(ptr, str, str_len);
	return ptr;
}
