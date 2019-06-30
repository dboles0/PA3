#include "fifo_queue.h"


// construct a queue with the provided max length
// initialize q params
// Time creation = O(1)
struct fifo_Q * create_queue(unsigned q_max){
	
	struct fifo_Q * queue = (struct fifo_Q*) malloc(sizeof(struct fifo_Q));
	queue->q_max = q_max;
	queue->q_size = 0;
	queue->head = -1;
	queue->tail = -1;
	queue->q_array = (char **) malloc(q_max * sizeof(char));
	queue->q_array[q_max] = NULL; 
	for(int i=0; i< queue->q_max; i++){
		queue->q_array[i] = copy("0");
	}
	queue->q_array[q_max] = NULL; 
	return queue;
}

// if return == 1 the queue is empty
// Time creation = O(1)
int q_is_empty(struct fifo_Q * a_queue){
	int is_empty = 0;

	if(a_queue->tail == -1 && a_queue->head == -1){
		is_empty = 1;			
	}
	return is_empty;
}

// if return == 1 the queue is full
// Time creation = O(1)
int q_is_full(struct fifo_Q * a_queue){
	int is_full = 0;

	if(((a_queue->tail + 1)%a_queue->q_max) == (a_queue->head)){
		is_full = 1;			
	}
	return is_full;
}

// enqueue add to the queue
// Time creation = O(1)
int enqueue(struct fifo_Q * a_queue, char * entry){
	
	if(q_is_full(a_queue)){
		printf("cannot add entry (%s)- queue is full\n", entry);
		return 0;
	}
	else if(q_is_empty(a_queue)){
		a_queue->head = 0;
		a_queue->tail = 0;
	}
	else {
		a_queue->tail = ((a_queue->tail + 1) % a_queue->q_max);
	}
	a_queue->q_array[a_queue->tail] = copy(entry);
	a_queue->q_size = a_queue->q_size + 1;
	return 1;
}

// dequeue remove item form the queue
// Time creation = O(1)
void dequeue(struct fifo_Q * a_queue){

	if(q_is_empty(a_queue)){
		return;
	}
	// if head == tail mark queue as empty
	else if(a_queue->head == a_queue->tail){
		a_queue->q_array[a_queue->head] = 0;
		a_queue->q_size = 0;
		a_queue->head = -1;
		a_queue->tail = -1;
	}
	else{
		free(a_queue->q_array[a_queue->head]);
		a_queue->q_array[a_queue->head] = copy("0");
		a_queue->head = ((a_queue->head + 1) % a_queue->q_max);
		a_queue->q_size = a_queue->q_size - 1;
	}
}

char * copy(char * str){
	int str_len = strlen(str);
	char * ptr = (char *) malloc (str_len + 1 * sizeof(char));
	memcpy(ptr, str, str_len);
	return ptr;
}

void clear_element(char * str){
	int str_len = strlen(str);
	memset(str, 1, str_len);
}
