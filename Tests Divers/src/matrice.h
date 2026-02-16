#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct mat_s {
    size_t height;
    size_t width;
    double *data;
} Mat;

Mat* mat_create(size_t height, size_t width, ...);
void mat_free(Mat *m);
Mat *mat_mult(Mat *a, Mat *b);
Mat *mat_mult_apply(Mat *a, Mat *b);
void mat_print(Mat *m);