// C program for array implementation of queue
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE_NAME_LENGTH 256

typedef struct
{
    int src_fd;
    int dest_fd;
    char src_file[MAX_FILE_NAME_LENGTH];
    char dest_file[MAX_FILE_NAME_LENGTH];
} FileEntry;

// A structure to represent a queue
struct Queue {
	int front, rear, size;
	unsigned capacity;
	FileEntry* array;
};

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
	struct Queue* queue = (struct Queue*)malloc(
		sizeof(struct Queue));
	queue->capacity = capacity;
	queue->front = queue->size = 0;

	// This is important, see the enqueue
	queue->rear = capacity - 1;
	queue->array = (FileEntry*)malloc(
		queue->capacity * sizeof(FileEntry));
	return queue;
}

// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue* queue)
{
	return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
	return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
int enqueue(struct Queue* queue, FileEntry item)
{
	if (isFull(queue))
		return 1;
	queue->rear = (queue->rear + 1)
				% queue->capacity;
	queue->array[queue->rear] = item;
	queue->size = queue->size + 1;
	return 0;
}

// Function to remove an item from queue.
// It changes front and size
FileEntry dequeue(struct Queue* queue)
{
	FileEntry dummyEntry;
	dummyEntry.src_fd = -1;
	if (isEmpty(queue))
		return dummyEntry;
	FileEntry item = queue->array[queue->front];
	queue->front = (queue->front + 1)
				% queue->capacity;
	queue->size = queue->size - 1;
	return item;
}


void destroy_queue(struct Queue* queue){
	free(queue->array);
}