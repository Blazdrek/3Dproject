#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

int main(){
    int* f = malloc(sizeof(int) * 3);
    f = realloc(f,0);
    if (f==NULL) printf("C'est NULL");
    else printf("C'est Bien");
}