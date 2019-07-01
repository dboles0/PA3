#include "multi-lookup.h"
#include "test.h"

int main(int argc, char *argv[]){

	int queue_max = 10;
	pthread_attr_t attr;                          // thread attribute
	struct fifo_Q * q;	
	pthread_t thread1;                                // hold id of thread
	struct thread_info * t_info;                    // pointer to sturcture

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
	enqueue(q, "test");
	//test_cases(q, queue_max);

	/*
	*  create container for threads
	*/
	// allocate space for thread info pointer
	int n = 10;
	t_info = (struct thread_info *)malloc(n * sizeof(t_info));
	t_info[0].file_name = malloc(strlen(argv[1]) + 1); 
	strncpy(t_info[0].file_name, argv[1], strlen(argv[1])); 
	memcpy(t_info[0].Q, q, sizeof(*(t_info[0].Q))); 

	// crate contaner for thread info
	//t_thread = create_thread_info();
	//memcpy(&t_thread->file_name, argv[1], (strlen(argv[1]) * sizeof(char)));

	// initiate thread
	pthread_attr_init(&attr);

	// add info for thread
	pthread_create(&thread1, &attr, parser_thread, (void *) &(t_info));	


	pthread_join(thread1, NULL);
	free(t_info);
	// print total runtime of pressesses to standard output using gettimeofday()
	
	return 0;

			
} 
