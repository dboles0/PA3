#include "test.h"
//#include "multi-lookup.h"

int main(int argc, char *argv[]){

	int queue_max = 10;
	struct fifo_Q * q = create_queue(queue_max);
	test_cases(q, queue_max);
	for (int i=0; i< argc; i++){
		printf("%s\n", argv[i]);
	}

	return 0;

			
} 
