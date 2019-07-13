#include "multi-lookup.h"
#include "test.h"

pthread_mutex_t q_lock = PTHREAD_MUTEX_INITIALIZER;	
pthread_mutex_t psr_log_lock = PTHREAD_MUTEX_INITIALIZER;	
pthread_mutex_t conv_log_lock = PTHREAD_MUTEX_INITIALIZER;	
pthread_mutex_t multi_file_lock = PTHREAD_MUTEX_INITIALIZER;	

pthread_cond_t needs_less = PTHREAD_COND_INITIALIZER;
pthread_cond_t needs_more = PTHREAD_COND_INITIALIZER;
bool done = false;
int files_serviced = 0;

int main(int argc, char *argv[]){

	int queue_max = 10;
	int num_input_files = argc - 1;
	struct fifo_Q * q;	
	FILE * in_files_fp[num_input_files];
	FILE * psr_log_fp;
	FILE * conv_log_fp;
	pthread_attr_t p_attr[num_input_files];          // thread attribute
	pthread_attr_t c_attr[num_input_files];          // thread attribute
	pthread_t p_threads[num_input_files];            // hold id of thread
	pthread_t c_threads[num_input_files];            // hold id of thread
	struct parser_info * psr_info;                   // pointer to sturcture
	struct converter_info * conv_info;               // pointer to sturcture
	struct timeval start, end;


	gettimeofday(&start, NULL);	

	/*
	*  check arguments
	*/
	
	// read parameters form the command line(section 4.1) with names
	if(num_input_files < MIN_ARGS){
		printf("error - at least one file needed\n");
		return 0;
	}
	if (num_input_files > MAX_INPUT_FILES){
		printf("error - you entered to many file's\n");
		return 0;
	}

	/*
	*  process names and create thread pool
	*/

	printf("\n\"%s\" passsing files ", argv[0]);
	for(int i=1; i< argc; i++){
		printf("[%s]", argv[i]);
	}

	printf(" to thread pool...\n\n");	
	printf("num input files and threads = %d\n\n", num_input_files);

	/*
	*  create queue 
	*/

	q = create_queue(queue_max);
	test_cases(q, queue_max);

	/*
	*  keep file open for parsing and converting logs
	*/

	psr_log_fp = fopen("parsing_log_file.txt", "w");
	conv_log_fp = fopen("results-ref.txt", "w");

	// open files recieved from usr
	for(int i=0; i<num_input_files; i++){
		in_files_fp[i] = fopen(concat("input/", argv[i + 1]), "r");
	}

	/*
	*  create threads
	*/

	pthread_mutex_init(&q_lock, NULL);
	pthread_mutex_init(&psr_log_lock, NULL);
	pthread_mutex_init(&conv_log_lock, NULL);
	pthread_mutex_init(&multi_file_lock, NULL);

	pthread_cond_init(&needs_less, NULL);
	pthread_cond_init(&needs_more, NULL);

	

	// create parser threads 
	psr_info = (struct parser_info *)malloc(MAX_CONVERT_THREADS * sizeof(psr_info));
	for(int psr=0; psr<num_input_files; psr++){
		
		// allocate space for thread info
		psr_info[psr].psr_log_fp = psr_log_fp;
		psr_info[psr].in_files_fp = in_files_fp[psr];
		psr_info[psr].num_files_serviced += 1;
		psr_info[psr].num_input_files = num_input_files;
		psr_info[psr].Q = malloc(sizeof(q)); 
		memcpy(&psr_info[psr].Q, &q, sizeof(psr_info[psr].Q)); 

		pthread_attr_init(&p_attr[psr]);
	
		//pthread_mutex_lock(&multi_file_lock);
		// crate new parser thread	
		pthread_create(&(p_threads[psr]), &p_attr[psr], parser_thread, (void *) &psr_info[psr]);	
		//pthread_mutex_unlock(&multi_file_lock);

	}
	// create converter threads
	for(int cov=0; cov<num_input_files; cov++){
		//pthread_mutex_lock(&lock);
		conv_info = (struct converter_info *)malloc(MAX_CONVERT_THREADS * sizeof(conv_info));
		conv_info[cov].conv_log_fp = conv_log_fp;
		conv_info[cov].Q = malloc(sizeof(q)); 
		memcpy(&conv_info[cov].Q, &q, sizeof(conv_info[cov].Q)); 
	
		// initiate conveter thread attribute
		pthread_attr_init(&c_attr[cov]);

		// crate new converter thread
		pthread_create(&(c_threads[cov]), &c_attr[cov], converter_thread, (void *) &conv_info[cov]);	
		//pthread_mutex_unlock(&lock);
	}

	for(int i=0; i<num_input_files; i++){
		pthread_join(p_threads[i], NULL);
		pthread_join(c_threads[i], NULL);
	}
	
	// close all open files	
	for(int i=0; i<num_input_files; i++){
		fclose(in_files_fp[i]);
	}
		
	fclose(psr_log_fp);	
	fclose(conv_log_fp);	
	

	//if(done) { free(psr_info); } 
	queue_state(q);
	delete_queue(q);
	// free all information instide the struct

	// print total runtime of pressesses to standard output using gettimeofday()

	
	gettimeofday(&end, NULL);

	printf("completion time = %ld seconds\n", (end.tv_sec - start.tv_sec));
	return 0;

			
} 
