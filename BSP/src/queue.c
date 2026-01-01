#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct queue_s {
    int start;
    int len;
    int MAXLEN;
    void** arr;
} queue;

queue* create_queue(){
    queue* q = malloc(sizeof(queue));
    q->start = 0;
    q->len = 0;
    q->MAXLEN = 10;
    q->arr = malloc(sizeof(void*) * 10);
    return q;
}

int len(queue* q){return q->len;}
int is_empty(queue* q) { return len(q) <= 0;}

int enqueue(queue* q,void* elt){

    if (len(q) == q->MAXLEN){
        void** new_arr = malloc(sizeof(void*)*q->MAXLEN*2);
        for (int i = 0;i < len(q);i++) new_arr[i] = q->arr[(q->start + i)%q->MAXLEN];
        q->start = 0;
        q->MAXLEN *= 2;
        free(q->arr);
        q->arr = new_arr;
    }
    q->len++;
    q->arr[(q->start + q->len - 1)%q->MAXLEN] = elt;
    
    return 0;
    
}

void sort_queue(queue* q,int comp(void* e1,void* e2)){
    for (int i = 0;i<q->len;i++){
        void* min = q->arr[i];
        int ind = i;
        for (int j = i + 1;j<q->len;i++){
            if (comp(min,q->arr[i]) > 0){
                min = q->arr[i];
                ind = j;
            }
        }
        q->arr[ind] = q->arr[i];
        q->arr[i] = min;
    }
}

void* dequeue(queue* q){
    assert(!is_empty(q));
        if (len(q) < q->MAXLEN/4){
        void** new_arr = malloc(sizeof(void*)*(q->MAXLEN/4 + 1));
        for (int i = 0;i < len(q); i++) new_arr[i] = q->arr[(q->start + i)%q->MAXLEN] ;
        free(q->arr);
        q->arr = new_arr;
        q->start = 0;
        q->MAXLEN = q->MAXLEN/4 + 1;
    }
    void* tmp = q->arr[q->start];
    q->start = (q->start + 1)%q->MAXLEN;
    q->len--;
    return tmp;
}

void _printQstate(queue* q,void print_elt(void* e1)){
    for (int i = 0;i<q->len;i++){
        print_elt(q->arr[(q->start + i)%q->MAXLEN]);
        printf(" ->");
    }
    printf("\n");
}

void free_queue(queue* q){
    free(q->arr);
    free(q);
}

void desintegrate(queue* q){
    for ( int i = 0;i< q->len;i++) free(q->arr[(q->start + i)%q->MAXLEN]);
    free_queue(q);
}