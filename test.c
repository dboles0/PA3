#include "test.h"

// test integer input in queue
void test_cases(struct fifo_Q * test_queue, int max){

	
	/*
	* Test enqueue
	*/
	
 	test1_enqueue(test_queue, max);
	test2_enqueue_overflow(test_queue, max);

	/*
	* Test dequeue
	*/
	test3_dequeue_one_element(test_queue, max);
	test4_dequeue_one_more_than_list(test_queue, max);
	test5_dequeue_wraparound(test_queue, max);
	test6_enqueue_dequeue_wraparound_overflow(test_queue, max);

	/*
	* Test threads
	*/


}

void queue_state(struct fifo_Q * queue){
	if(q_is_empty(queue)) { printf(" (making new array...)\n"); }
	printf("q_max = %i\n", queue->q_max);
	printf("head = %i\n", queue->head);
	printf("tail = %i\n", queue->tail);
	printf("q_size = %i\n", queue->q_size);
	//printf("total size of queue array = %lu\n", strlen(*(queue->q_array)));
	
	printf("\nqueue array = {\n");
	for (int i=0; i < queue->q_max; i++){
		if(queue->q_array[i] == '\0'){
			printf("\nerror - cannot access array[%d]\n", i);
		}
		else { printf("%s", queue->q_array[i]); }
	}
	
	printf("}\n\n");
}

void q_error_check(struct fifo_Q * test_queue, int defined_q_max, int size, int head, int tail, char ** test_array){

	int success = 1;

	if(test_queue->q_max != defined_q_max) { 
		success = 0; 
		printf("error - q_max = %i should be defined_q_max=%i\n", test_queue->q_max, defined_q_max); 
	}	
	if(test_queue->q_size != size) { 
		success = 0; 
		printf("error - q_size = %i should be size=%i\n", test_queue->q_size, size); 
	}
	if(test_queue->head != head) { 
		success = 0; 
		printf("error - head = %i should be %i\n", test_queue->head, head); 
	}
	if(test_queue->tail != tail) { 
		success = 0; 
		printf("error - tail = %i should be %i\n", test_queue->tail, tail); 
	}
	for (int i=0; i<test_queue->q_size-1; i++){
		if(strcmp(test_array[i], test_queue->q_array[i]) != 0){
			success = 0; 
			printf("error - array[%i] = %s should be [%s]\n", i, test_queue->q_array[i], test_array[i]);
		}
	}
	if(success == 1) { printf("\t-Test's Passed-\n"); }
	printf("\n");
}

void test1_enqueue(struct fifo_Q * test_queue, int max){

	char * test_array[10] = {"10", "20", "30" };
	int size, head, tail;
	char msg1[] = { "10" };
	char msg2[] = { "20" };
	char msg3[] = { "30" };


	size = head = tail = 0;
	size = 3;
	head = 0; 
	tail = 2;
	
	printf("\n\"testing enqueue for [10][20][30] (q_max =%i)\"\n", test_queue->q_max); 
	
	enqueue(test_queue, msg1);
	enqueue(test_queue, msg2); 
	enqueue(test_queue, msg3); 

	q_error_check(test_queue, max, size, head, tail, test_array);
}

void test2_enqueue_overflow(struct fifo_Q * test_queue, int max){

	char * test_array[10] = {"10", "20", "30", "1", "2", "3", "4", "5", "6", "7" };
	int size, head, tail;
	char msg1[] = { "1" };
	char msg2[] = { "2" };
	char msg3[] = { "3" };
	char msg4[] = { "4" };
	char msg5[] = { "5" };
	char msg6[] = { "6" };
	char msg7[] = { "7" };
	char msg8[] = { "8" };



	// Test out of bounds (to many) for enqueue should truncate 
	printf("\"testing enqueue*+8 (overflow - 1) [10][20][30][1][2][3][4][5][6][7] (q_max = %i)\"\n", test_queue->q_max); 

	enqueue(test_queue, msg1);
	enqueue(test_queue, msg2);
	enqueue(test_queue, msg3);
	enqueue(test_queue, msg4);
	enqueue(test_queue, msg5);
	enqueue(test_queue, msg6);
	enqueue(test_queue, msg7);
	enqueue(test_queue, msg8);

	size = head = tail = 0;
	size = 10;
	head = 0; 
	tail = max - 1;

	q_error_check(test_queue, max, size, head, tail, test_array);
}

void test3_dequeue_one_element(struct fifo_Q * test_queue, int max){

	char * test_array[10] = {"0", "20", "30", "1", "2", "3", "4", "5", "6", "7"};
	int size, head, tail;

	// dequeue one element = [0][20][30][40]
	printf("\"testing dequeue for [0][20][30][1][2][3][4][5][6][7] (q_size = %i)\"\n", max); 
	dequeue(test_queue);
	size = head = tail = 0;
	size = 9;
	head = 1; 
	tail = max - 1;
	
	q_error_check(test_queue, max, size, head, tail, test_array);
}

void test4_dequeue_one_more_than_list(struct fifo_Q * test_queue, int max){

	char * test_array[10] = {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0"};
	int size, head, tail;

	// dequeue one more than list holds = [0][0][0][0][0] 0
	printf("\"testing dequeue*max+1 (two dequeue over q_max) [0][0][0][0][0][0][0][0][0][0] (q_size = %i)\"\n", max); 
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);

	size = head = tail = 0;
	size = 0;
	head = -1; 
	tail = -1;

	q_error_check(test_queue, max, size, head, tail, test_array);
}

void test5_dequeue_wraparound(struct fifo_Q * test_queue, int max){

	char * test_array[10] = {"1", "2", "13", "14", "15", "16", "17", "18", "19", "20"};
	int size, head, tail;
	char msg1[] = { "1" };
	char msg2[] = { "2" };
	char msg3[] = { "3" };
	char msg11[] = { "11" };
	char msg12[] = { "12" };
	char msg13[] = { "13" };
	char msg14[] = { "14" };
	char msg15[] = { "15" };
	char msg16[] = { "16" };
	char msg17[] = { "17" };
	char msg18[] = { "18" };
	char msg19[] = { "19" };
	char msg20[] = { "20" };

	// ensure wraparound using enqueue*5; dequeue*3; enqueue*2
	printf("\"wraparound test - enqueue*5 dequeue*3 enqueue*2 = [1][2][13][14][15][16][17][18][19][20] (q_size = %i)\"\n", max); 
	enqueue(test_queue, msg11);
	enqueue(test_queue, msg12);
	enqueue(test_queue, msg13);
	enqueue(test_queue, msg14);
	enqueue(test_queue, msg15);
	dequeue(test_queue);
	dequeue(test_queue);
	enqueue(test_queue, msg16);
	enqueue(test_queue, msg17);
	enqueue(test_queue, msg18);
	enqueue(test_queue, msg19);
	enqueue(test_queue, msg20);
	enqueue(test_queue, msg1);
	enqueue(test_queue, msg2);
	enqueue(test_queue, msg3);

	size = head = tail = 0;
	size = 10;
	head = 2; 
	tail = 1;

	q_error_check(test_queue, max, size, head, tail, test_array);
}

void test6_enqueue_dequeue_wraparound_overflow(struct fifo_Q * test_queue, int max){

	char * test_array[10] = {"21", "22", "13", "14", "15", "16", "17", "18", "19", "20"};
	int size, head, tail;

	char msg11[] = { "11" };
	char msg12[] = { "12" };
	char msg13[] = { "13" };
	char msg14[] = { "14" };
	char msg15[] = { "15" };
	char msg16[] = { "16" };
	char msg17[] = { "17" };
	char msg18[] = { "18" };
	char msg19[] = { "19" };
	char msg20[] = { "20" };
	char msg21[] = { "21" };
	char msg22[] = { "22" };
	char msg23[] = { "23" };


	// ensure if wraparound overflow 
	printf("\"ensure truncate/wraparound (overwrite 12&13/overflow 23) (q_size = %i)\"\n", max); 
	// clear queue
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);
	dequeue(test_queue);

	// wraparound +1
	enqueue(test_queue, msg11);
	enqueue(test_queue, msg12);
	enqueue(test_queue, msg13);
	enqueue(test_queue, msg14);
	enqueue(test_queue, msg15);
	dequeue(test_queue);
	dequeue(test_queue);
	enqueue(test_queue, msg16);
	enqueue(test_queue, msg17);
	enqueue(test_queue, msg18);
	enqueue(test_queue, msg19);
	enqueue(test_queue, msg20);
	enqueue(test_queue, msg21);
	enqueue(test_queue, msg22);
	enqueue(test_queue, msg23);

	size = head = tail = 0;
	size = 10;
	head = 2; 
	tail = 1;

	q_error_check(test_queue, max, size, head, tail, test_array);
}

