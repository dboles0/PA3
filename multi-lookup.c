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
	int i;
	
	char str[MAX_NAME_LENGTH];                         // string read form file

	// save thread Id 
	a_psr_info.thread_id = pthread_self();

	// pthread_mutex_lock(&lock);
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

	FILE * fp;
	struct converter_info a_conv_info = *((struct converter_info *) ptr);
	char ipstr[MAX_IP_LENGTH];                                             // ip address
	char * domain; 
	a_conv_info.thread_id = pthread_self();

	
	fp = fopen("results-ref.txt", "w");
	while(!done){
		while(a_conv_info.Q->q_size > 0){
			pthread_mutex_lock(&lock);
			// if queue is empty block until new item or all parser threads have terminated
			while(q_is_empty(a_conv_info.Q) == 1){
				pthread_cond_wait(&needs_more, &lock);
			}

			domain = a_conv_info.Q->q_array[a_conv_info.Q->head];
			// query the IP Address
			if(dnslookup(domain, ipstr, strlen(domain)) == UTIL_FAILURE){
				strncpy(ipstr, "", sizeof(ipstr));
			}
			printf("%s:%s\n", domain, ipstr);
	
			fprintf(fp, "%s,%s\n", domain, ipstr);
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

