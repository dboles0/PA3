#include "fifo_queue.h"

struct q_node {
	char * data;
	struct fifo_Q * next;
};

struct fifo_Q{
	struct q_node * root, * tail;
};

struct q_node * new_node(char * to_add){

	struct q_node * temp = (struct q_nod*) malloc(sizeof(struct q_node));
	temp->data = to_add;
	temp->next = NULL;
	return temp;
}

void enqueue(struct fifo_Q * a_queue, char &* to_add){

	struct q_node * temp = new_node(to_add);

	// if queue is empty point root and tail to temp
	if(q_is_empty(a_queue)){
		a_queue->root = a_queue->tail = temp; 
		return;
	}
	// root ... tail->next ---|
	// [x]  ->  [x]    ->   [temp] 
	// root ................  tail
	// connect the tail of list to new_node
	a_queue->tail->next = temp;
	a_queue->tail = temp;
}

void dequeue(struct fifo_Q * a_queue){
	if(q_is)
}

int q_is_empty(struct fifo_Q * a_queue){

	int is_empty = 0;
	if(a_queue->rear == NULL){
		is_empty = 1;
	}
	return is_empty;
}

int q_is_full(struct fifo_Q * a_queue){
}

