#include "stack.h"

typedef struct stack_s {
    int len;
    void** elements;
    int MAXLEN;
} stack;


stack* STACK_init(){
    stack *s = malloc(sizeof(stack));
    s->len = 0;
    s->MAXLEN = 10;
    s->elements = malloc(sizeof(void*)*s->MAXLEN);
    return s;
}

int STACK_len(stack *s){ return s->len; }

int STACK_isEmpty(stack *s){ return !(s->len > 0);}

void STACK_add(stack *s,void* elt){
    if (s->len == s->MAXLEN){
        s->MAXLEN *= 2;
        s->elements = realloc(s->elements,sizeof(void*)*s->MAXLEN);
    }
    s->elements[s->len] = elt;
    s->len++;
    return;
}

void* STACK_pop(stack *s){
    assert(!STACK_isEmpty(s));
    s->len--;
    return s->elements[s->len];
}

void STACK_free(stack *s){
    free(s->elements);
    free(s);
}       

void STACK_desintegrate(stack *s){
    for (int i = 0;i < s->len;i++){
        free(s->elements[i]);
    }
    STACK_free(s);
}