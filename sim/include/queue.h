#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdlib.h>

typedef struct QueueNode {
	void *val;
	struct QueueNode *next;
	struct QueueNode *prev;
} QueueNode;

typedef struct {
	QueueNode *front;
	QueueNode *back;
	size_t len;
	size_t size;
} Queue;
Queue MakeEmptyQueue(size_t size);
void QueuePush(Queue *queue, void *elem);
void QueuePop(Queue *queue);

#endif
