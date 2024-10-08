#include "op_queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

OpQueue *MakeEmptyOpQueue() { return calloc(1, sizeof(OpQueue)); }

void OpQueuePush(OpQueue *queue, Operation op) {
	if (queue->len == 0) {
		queue->front = calloc(1, sizeof(QueueNode));
		queue->front->op = op;
		queue->back = queue->front;
	} else {
		QueueNode *p = calloc(1, sizeof(QueueNode));
		p->prev = queue->back;
		queue->back->next = p;
		queue->back = p;
	}
	++queue->len;
}

void OpQueuePop(OpQueue *queue) {
	if (queue->len == 0) return;

	QueueNode *tmp = queue->front;

	if (queue->len == 1) {
		queue->front = NULL;
		queue->back = NULL;
	} else {
		queue->front = queue->front->next;
		queue->front->prev = NULL;
	}

	if (tmp->op.data != NULL) free(tmp->op.data);
	free(tmp);
	--queue->len;
}

const char *BumpQueue(OpQueue *queue) {
	if (queue->len == 0) return "";

	char *nodeStrs[queue->len];
	QueueNode *node = queue->front;
	for (size_t i = 0; i < queue->len; ++i) {
		sprintf(nodeStrs[i], "%d", node->op.type);
		node = node->next;
	}
	return strsep(nodeStrs, ",");
}
