#include <math.h>
#include <stdio.h>
#include <assert.h>
#define PI 3.14159265359

double sinus(double x,int precision){
    double x2 = x*x;
    double result = x;
    double current_x = x;
    for (double i = 0.0;i< precision;i++){
        double denom = (2*(i+1))*(2*(i+1)+1);
        current_x = current_x * (-1.0) * x2 / denom;
        result = result + current_x;
    }
    return result;
}
double cosinus(double x,int precision){
    return sinus(PI/2 - x,precision);
}

double arcsin_v1(double x,int precision){
    assert(x<=1.0);
    double x2 = x*x;
    double result = x;
    double current_x = x;
    for (double i = 0.0;i<precision;i++){
        double i2 = (2*i + 1.0)*(2*i + 1.0);
        double i2p1 = 2*(i + 1.0);
        current_x = current_x *i2*x2/ (i2p1 * (i2p1 + 1.0));
        result = result + current_x;
    }
    return result;
}

double arccos_v1(double x,int precision){
    return PI/2 - arcsin_v1(x,precision);
}

int main(){
    double v = PI/3;
    printf("Valeur        : %f \n",v);
    double x1 = sin(v);
    double x2 = sinus(v,10);
    printf(" Sinus Math.h : %f \n       Moi    : %f \n",x1,x2);
    printf(" Cosin Math.h : %f \n       Moi    : %f \n",cos(v),cosinus(v,10
    ));
    double y1 = asin(x1);
    double y2 = arcsin_v1(x2,10);
    printf("Arcsin Math.h : %f \n       Moi    : %f \n ",y1,y2);
    return 0;


}