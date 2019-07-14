#include "multi-lookup.h"
#include "test.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;	
pthread_mutex_t psr_log_lock = PTHREAD_MUTEX_INITIALIZER;	
pthread_mutex_t test_lock = PTHREAD_MUTEX_INITIALIZER;	
pthread_cond_t needs_less = PTHREAD_COND_INITIALIZER;
pthread_cond_t needs_more = PTHREAD_COND_INITIALIZER;
bool done = false;
int files_serviced = 0;

int main(int argc, char *argv[]){

	int queue_max = 10;
	int num_input_files = argc - 1;
	struct fifo_Q * q;
	FILE * psr_log_fp;	
	FILE * usr_input_fps[num_input_files];	
	pthread_attr_t p_attr[num_input_files];          // thread attribute
	pthread_attr_t c_attr;                           // thread attribute
	pthread_t p_threads[num_input_files];            // hold id of thread
	pthread_t c_threads;                             // hold id of thread
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

	/*
	*  create queue 
	*/

	q = create_queue(queue_max);
	//test_cases(q, queue_max);

	/*
	*  create threads
	*/

	pthread_mutex_init(&lock, NULL);
	pthread_mutex_init(&psr_log_lock, NULL);
	pthread_mutex_init(&test_lock, NULL);
	pthread_cond_init(&needs_less, NULL);
	pthread_cond_init(&needs_more, NULL);

	// inititate all threads

	// open all input files 
	psr_log_fp = fopen("parsing_log_file.txt", "w");
	for(int i=0; i<num_input_files; i++){
		usr_input_fps[i] = fopen(concat("input/", argv[i+1]), "r");
	}
	// create parser threads 
	for(int psr=0; psr<num_input_files; psr++){
		
		//pthread_mutex_lock(&lock);
		// allocate space for thread info
		psr_info = (struct parser_info *)malloc(MAX_CONVERT_THREADS * sizeof(psr_info));
		psr_info[psr].num_files_serviced += 1;
		psr_info[psr].num_input_files = num_input_files;
		psr_info[psr].psr_log_fp = psr_log_fp; 
		psr_info[psr].fp = usr_input_fps[psr]; 
		psr_info[psr].Q = malloc(sizeof(q)); 
		memcpy(&psr_info[psr].Q, &q, sizeof(psr_info[psr].Q)); 

		pthread_attr_init(&p_attr[psr]);
	
		// crate new parser thread	
		pthread_create(&(p_threads[psr]), &p_attr[psr], parser_thread, (void *) &psr_info[psr]);	
		//pthread_mutex_unlock(&lock);

	}
	// create converter threads
	for(int cov=0; cov<1; cov++){
		//pthread_mutex_lock(&lock);
		conv_info = (struct converter_info *)malloc(MAX_CONVERT_THREADS * sizeof(conv_info));
		conv_info[cov].Q = malloc(sizeof(q)); 
		memcpy(&conv_info[cov].Q, &q, sizeof(conv_info[cov].Q)); 
	
		// initiate conveter thread attribute
		pthread_attr_init(&c_attr);

		// crate new converter thread
		pthread_create(&c_threads, &c_attr, converter_thread, (void *) &conv_info[cov]);	
		//pthread_mutex_unlock(&lock);
	}
		
	for(int i=0; i<num_input_files; i++){
		pthread_join(p_threads[i], NULL);
	}


	pthread_join(c_threads, NULL);

	// close file pointrs
	fclose(psr_log_fp);
	for(int i=0; i<num_input_files; i++){
		fclose(usr_input_fps[i]);
	}

	queue_state(q);
	delete_queue(q);
	//free(psr_info);

	// print total runtime of pressesses to standard output using gettimeofday()
	pthread_mutex_destroy(&lock);
	pthread_mutex_destroy(&test_lock);

	gettimeofday(&end, NULL);
	printf("completion time = %ld seconds\n", (end.tv_sec - start.tv_sec));	
	return 0;

			
} 
