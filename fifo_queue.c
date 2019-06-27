#include "fifo_queue.h"


// construct a queue with the given capacity
// initialize q params
// Time creation = O(1)
struct fifo_Q * create_queue(unsigned q_max){
	
	struct fifo_Q * queue = (struct fifo_Q*) malloc(sizeof(struct fifo_Q));
	queue->q_max = q_max;
	queue->head = -1;
	queue->tail = -1;
	queue->q_array = (int*) malloc(queue->q_max * sizeof(int));
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
void enqueue(struct fifo_Q * a_queue, int entry){

	if(q_is_full(a_queue)){
		printf("cannot add entry (%i)- queue is full\n", entry);
		return;
	}
	else if(q_is_empty(a_queue)){
		a_queue->head = 0;
		a_queue->tail = 0;
	}
	else {
		a_queue->tail = ((a_queue->tail + 1) % a_queue->q_max);
	}
	a_queue->q_array[a_queue->tail] = entry;
	a_queue->q_size = a_queue->q_size + 1;
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
		a_queue->q_array[a_queue->head] = 0;
		a_queue->head = ((a_queue->head + 1) % a_queue->q_max);
		a_queue->q_size = a_queue->q_size - 1;
	}
}

