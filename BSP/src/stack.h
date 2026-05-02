#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct stack_s stack;
stack* STACK_init();
int STACK_len(stack* s);
int STACK_isEmpty(stack* s);
void STACK_add(stack* s,void* elt);
void* STACK_pop(stack* s);
void STACK_free(stack* s);
void STACK_desintegrate(stack *s);

