#include "multi-lookup.h"
#include "test.h"


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;	
pthread_cond_t needs_less = PTHREAD_COND_INITIALIZER;
pthread_cond_t needs_more = PTHREAD_COND_INITIALIZER;
bool done = false;

int main(int argc, char *argv[]){

	int queue_max = 10;
	pthread_attr_t attr;                          // thread attribute
	pthread_attr_t attr1;                          // thread attribute
	struct fifo_Q * q;	
	pthread_t thread1;                                // hold id of thread
	pthread_t thread2;                                // hold id of thread
	struct parser_info * psr_info;                    // pointer to sturcture
	struct converter_info * conv_info;                    // pointer to sturcture

	/*
	*  check arguments
	*/
	
	// read parameters form the command line(section 4.1) with names
	if(argc-1 < MIN_ARGS){
		printf("error - at least one file needed\n");
		return 0;
	}
	if (argc-1 > MAX_INPUT_FILES){
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
	*  create container for threads
	*/
	// allocate space for thread info pointer

	// fifo_q  * q { int q_max; int q_size; int q_head; int q_tail; char ** q_array } 
	// thread_info * t_thread;
	// thread_info  { fifo_q * Q; char * file_name } 

	// parser thread pool
	psr_info = (struct parser_info *)malloc(MAX_CONVERT_THREADS * sizeof(psr_info));
	psr_info[0].num_files_serviced += 1;
	psr_info[0].file_name = (char *) malloc(strlen(argv[1]) * sizeof(char));
	strncpy(psr_info[0].file_name, argv[1], strlen(argv[1])); 
	psr_info[0].Q = malloc(sizeof(q)); 
	memcpy(&psr_info[0].Q, &q, sizeof(psr_info[0].Q)); 

	// converter thread pool
	conv_info = (struct converter_info *)malloc(MAX_CONVERT_THREADS * sizeof(conv_info));
	conv_info[0].Q = malloc(sizeof(q)); 
	memcpy(&conv_info[0].Q, &q, sizeof(conv_info[0].Q)); 

	// crate contaner for thread info

	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&needs_less, NULL);
	pthread_cond_init(&needs_more, NULL);

	// initiate thread
	pthread_attr_init(&attr);
	pthread_attr_init(&attr1);

	// add info for thread
	pthread_create(&thread1, &attr, parser_thread, (void *) &psr_info[0]);	
	pthread_create(&thread2, &attr1, converter_thread, (void *) &conv_info[0]);	


	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	free(psr_info);
	queue_state(q);
	// print total runtime of pressesses to standard output using gettimeofday()
	
	return 0;

			
} 
