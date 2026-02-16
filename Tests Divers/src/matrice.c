#include "matrice.h"


/// @brief Create a matrix with given height and width, initialized with 0.
/// @param height 
/// @param width 
/// @param  ... float values to initialize the matrix
/// @return
Mat* mat_create_empty(size_t height, size_t width, ...) {
    Mat *res = malloc(sizeof(Mat));
    if (res == NULL) return NULL;
    res->height = height;
    res->width = width;

    res->data = calloc(height * width, sizeof(double));
    if (res->data == NULL) {
        free(res);
        return NULL;
    }
    
    return res;
}


/// @brief Create a matrix with given height and width, initialized with float values.
/// @param height 
/// @param width 
/// @param  ... float values to initialize the matrix
/// @return 
Mat* mat_create(size_t height, size_t width, ...) {
    Mat *res = malloc(sizeof(Mat));
    if (res == NULL) return NULL;
    res->height = height;
    res->width = width;

    res->data = malloc(sizeof(double) * height * width);
    if (res->data == NULL) {
        free(res);
        return NULL;
    }

    va_list ap;
    va_start(ap, width);
    for (int i = 0; i < height * width; i++)
    {
        res->data[i] = va_arg(ap, double);
    }
    va_end(ap);
    
    return res;
}

void mat_free(Mat *m) {
    free(m->data);
    free(m);
}

Mat *mat_mult(Mat *a, Mat *b) {
    if (a == NULL || b == NULL) return NULL;
    if (a->width != b->height) return NULL;

    Mat *res = mat_create_empty(a->height, b->width);
    for (size_t y = 0; y < a->height; y++)
    {
        for (size_t x = 0; x < b->width; x++)
        {
            res->data[y * b->width+ x] = 0;
            for (size_t i = 0; i < a->width; i++)
            {
                res->data[y * b->width + x] += a->data[y * a->height + i] * b->data[i * b->width + x];
            }
            
        }
        
    }
    return res;
}

Mat *mat_mult_apply(Mat *a, Mat *b) {
    if (a == NULL || b == NULL) return NULL;
    Mat* mult = mat_mult(a, b);
    free(b->data);
    b->data = mult->data;
    free(mult);
    return b;
}

void mat_print(Mat *m) {
    for (size_t i = 0; i < m->height; i++)
    {
        for (size_t j = 0; j < m->width; j++)
        {
            printf("%.3f ", m->data[i*m->width + j]);
        }
        printf("\n");
        fflush(stdout);
    }
    
}