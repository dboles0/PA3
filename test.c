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

	for (int i=0; i < queue->q_max; i++){
		printf("[%i]", queue->q_array[i]);
	}
	printf("\n\n");
}

void q_error_check(struct fifo_Q * test_queue, int defined_q_max, int size, int head, int tail, int * test_array){

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
	for (int i=0; i<defined_q_max; i++){
		if(test_array[i] != test_queue->q_array[i]){
			success = 0; 
			printf("error - array[%i] = %i\n", i, test_queue->q_array[i]);
		}
	}
	if(success == 1) { printf("\t-Test's Passed-\n"); }
	printf("\n");
}

void test1_enqueue(struct fifo_Q * test_queue, int max){

	int test_array[10] = {20, 30, 40, 0, 0, 0, 0, 0, 0, 0};
	int size, head, tail;

	size = head = tail = 0;
	size = 3;
	head = 0; 
	tail = 2;
	
	printf("\n\"testing enqueue for [20][30][40] (q_size =%i)\"\n", max); 
	
	enqueue(test_queue, 20);
	enqueue(test_queue, 30);
	enqueue(test_queue, 40);

	q_error_check(test_queue, max, size, head, tail, test_array);
}

void test2_enqueue_overflow(struct fifo_Q * test_queue, int max){

	int test_array[10] = {20, 30, 40, 1, 2, 3, 4, 5, 6, 7};
	int size, head, tail;

	// Test out of bounds (to many) for enqueue should truncate 
	printf("\"testing enqueue*5 (overflow - 3, 4, 5) [20][30][40][1][2][3][4][5][6][7] (q_size = %i)\"\n", max); 
	enqueue(test_queue, 1);
	enqueue(test_queue, 2);
	enqueue(test_queue, 3);
	enqueue(test_queue, 4);
	enqueue(test_queue, 5);
	enqueue(test_queue, 6);
	enqueue(test_queue, 7);
	enqueue(test_queue, 8);

	size = head = tail = 0;
	size = 10;
	head = 0; 
	tail = max - 1;

	q_error_check(test_queue, max, size, head, tail, test_array);
}

void test3_dequeue_one_element(struct fifo_Q * test_queue, int max){

	int test_array[10] = {0, 30, 40, 1, 2, 3, 4, 5, 6, 7};
	int size, head, tail;

	// dequeue one element = [0][20][30][40]
	printf("\"testing dequeue for [X][30][40][1][2][3][4][5][6][7] (q_size = %i)\"\n", max); 
	dequeue(test_queue);
	size = head = tail = 0;
	size = 9;
	head = 1; 
	tail = max - 1;

	q_error_check(test_queue, max, size, head, tail, test_array);
}

void test4_dequeue_one_more_than_list(struct fifo_Q * test_queue, int max){

	int test_array[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int size, head, tail;

	// dequeue one more than list holds = [x][x][x][x][x] x
	printf("\"testing dequeue*max+1 (two dequeue over q_max) [X][X][X][X][X][X][X][X][X][X] (q_size = %i)\"\n", max); 
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

	int test_array[10] = {1, 2, 13, 14, 15, 16, 17, 18, 19, 20};
	int size, head, tail;

	// ensure wraparound using enqueue*5; dequeue*3; enqueue*2
	printf("\"ensure wraparound = [1][2][13][14][15][16][17][18][19][20] (q_size = %i)\"\n", max); 
	enqueue(test_queue, 11);
	enqueue(test_queue, 12);
	enqueue(test_queue, 13);
	enqueue(test_queue, 14);
	enqueue(test_queue, 15);
	dequeue(test_queue);
	dequeue(test_queue);
	enqueue(test_queue, 16);
	enqueue(test_queue, 17);
	enqueue(test_queue, 18);
	enqueue(test_queue, 19);
	enqueue(test_queue, 20);
	enqueue(test_queue, 1);
	enqueue(test_queue, 2);
	enqueue(test_queue, 3);

	size = head = tail = 0;
	size = 10;
	head = 2; 
	tail = 1;

	q_error_check(test_queue, max, size, head, tail, test_array);
}

void test6_enqueue_dequeue_wraparound_overflow(struct fifo_Q * test_queue, int max){

	int test_array[10] = {21, 22, 13, 14, 15, 16, 17, 18, 19, 20};
	int size, head, tail;

	// ensure if wraparound overflow 
	printf("\"ensure wraparound truncate (overflow 18) (q_size = %i)\"\n", max); 
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
	enqueue(test_queue, 11);
	enqueue(test_queue, 12);
	enqueue(test_queue, 13);
	enqueue(test_queue, 14);
	enqueue(test_queue, 15);
	dequeue(test_queue);
	dequeue(test_queue);
	enqueue(test_queue, 16);
	enqueue(test_queue, 17);
	enqueue(test_queue, 18);
	enqueue(test_queue, 19);
	enqueue(test_queue, 20);
	enqueue(test_queue, 21);
	enqueue(test_queue, 22);

	size = head = tail = 0;
	size = 10;
	head = 2; 
	tail = 1;

	q_error_check(test_queue, max, size, head, tail, test_array);
}

/*
void test_open_file(void){

	int num_files = 0, buffer_size = 5;
	char * file_names[buffer_size];	
	char * t_file_names[5] = {"names1.txt", "names2.txt", "names4.txt", "names5.txt", "names3.txt", NULL};

	printf("\"get array of file names from the input dir\"\n"); 

	//get_file_names(file_names, &num_files, &buffer_size);	

	int flag = 0;
	for (int i=0; i<num_files;i++){
		if(strcmp(file_names[i], t_file_names[i]) == 0){ flag = 1; }
	}
	if (flag == 1){ printf("\t-Test's Passed-\n"); }
	else { printf("directory open - error\n"); }
}
*/
