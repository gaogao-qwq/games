#include "op_queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strjoin(char *restrict dst, const char **restrict strs, size_t len,
              const char *restrict delim) {
	if (dst == NULL) return dst;
	size_t i;
	if (delim == NULL || !strlen(delim)) {
		for (i = 0; i < len; ++i) {
			strcat(dst, strs[i]);
		}
		return dst;
	}

	for (i = 0; i < len; ++i) {
		strcat(dst, strs[i]);
		if (i < len - 1) strcat(dst, delim);
	}
	return dst;
}

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

void OpQueueBump(OpQueue *queue) {
	if (queue->len == 0) return;
	QueueNode *node = queue->front;
	fprintf(stdout, "Bumping operation queue:\n");
	for (size_t i = 0; i < queue->len; ++i) {
		fprintf(stdout, "\top[%lu]: %d\n", i, node->op.type);
		node = node->next;
	}
}
