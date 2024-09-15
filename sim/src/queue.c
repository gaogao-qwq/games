#include "../include/queue.h"

#include <stdlib.h>

Queue MakeEmptyQueue(size_t size) { return (Queue){NULL, NULL, 0, size}; }

void QueuePush(Queue *queue, void *elem) {
	if (queue->len == 0) {
		queue->front = calloc(1, sizeof(QueueNode));
		queue->front->val = elem;
		queue->back = queue->front;
	} else {
		QueueNode *p = calloc(1, sizeof(QueueNode));
		queue->back->next = p;
		p->prev = queue->back;
		queue->back = p;
	}
	++queue->len;
}

void QueuePop(Queue *queue) {
	if (queue->len == 0) {
		return;
	}
	QueueNode *tmp = queue->front;
	queue->front = queue->front->next;
	if (queue->front != NULL) {
		queue->front->prev = NULL;
	}
	free(tmp);
	--queue->len;
}
