#include <stdio.h>


int main(){

    for (double i = 0;i<=1;i++){
        for (double j = 0;j<=1;j++){
            int x = ((int)i+(int)j+1)%2;
            int y = (((int)i+(int)j)%2)*3 + (((int)i+(int)j+1)%2)*2;
            printf("(%d,%d)  |  ",x,y);
        }
    }

    return 0;
}