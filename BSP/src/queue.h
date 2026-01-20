#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue_s queue;
queue* create_queue();
int enqueue(queue* q,void* elt);
int is_empty(queue* q);
int len(queue* q);
void* dequeue(queue* q);
void free_queue(queue* q);
void desintegrate(queue* q);
void _printQstate(queue* q,void print_elt(void* e1));
void sort_queue(queue* q,int comp(void* e1,void* e2));




#endif