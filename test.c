#include <stdio.h>


int main(){
    unsigned long int i = 1;
    unsigned long last = 0;
    int p = 1;
    while (i>0 && last!=i){
        p++;
        last = i;
        printf("(%lu = 2^%d - 1)\n" ,i,p);
        i = (i+1)*2 - 1;
        
        
    }

    return 0;
}