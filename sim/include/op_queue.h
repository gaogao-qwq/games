#ifndef OP_QUEUE_H_
#define OP_QUEUE_H_
#include <stdlib.h>

typedef enum {
	OP_BRUSH_SWITCH,
	OP_BRUSH_DRAW,
} OperationType;

typedef struct {
	int type;
	void *data;
} Operation;

typedef struct QueueNode {
	Operation op;
	struct QueueNode *next;
	struct QueueNode *prev;
} QueueNode;

typedef struct {
	QueueNode *front;
	QueueNode *back;
	size_t len;
} OpQueue;
OpQueue *MakeEmptyOpQueue();
void OpQueuePush(OpQueue *queue, Operation op);
void OpQueuePop(OpQueue *queue);
const char *BumpQueue(OpQueue *queue);

#endif
