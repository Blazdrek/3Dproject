#include "player.h"
#include "interface.h"
#include "matrice.h"

int main(int argc, char const *argv[])
{
    Mat *A = mat_create(3, 3, 
        1.0, 2.0, 3.0, 
        4.0, 5.0, 6.0, 
        7.0, 8.0, 9.0);
    Mat *B = mat_create(3, 3, 
        4.0, 5.0, 6.0, 
        1.0, 2.0, 3.0, 
        7.0, 8.0, 9.0);

    mat_print(mat_mult_apply(A, B));

    return 0;
}
