#include "fifo_queue.h"

void test_cases(struct fifo_Q * test_queue, int q_max);
void test1_enqueue(struct fifo_Q * test_queue, int max);
void test2_enqueue_overflow(struct fifo_Q * test_queue, int max);
void test3_dequeue_one_element(struct fifo_Q * test_queue, int max);
void test4_dequeue_one_more_than_list(struct fifo_Q * test_queue, int max);
void test5_dequeue_wraparound(struct fifo_Q * test_queue, int max);
void test6_enqueue_dequeue_wraparound_overflow(struct fifo_Q * test_queue, int max);
void test_open_file(void);
void q_error_check(struct fifo_Q * test_queue, int defined_q_max, int size, int head, int tail, int * test_array);
void queue_state(struct fifo_Q * test_queue);


