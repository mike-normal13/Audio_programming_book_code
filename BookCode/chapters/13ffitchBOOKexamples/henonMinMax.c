#include <stdio.h>

// extern double A, B;

double A = 1.3, B = 0.21;


void henon(double *x, double *y)
{
    double new_x;
    double new_y;
    double xx = *x;
    double yy = *y;

    new_x = 1.0 + yy  - A * (xx*xx);
    new_y = B * xx;
    *x = new_x;
    *y = new_y;
}


int main(int argc, char **argv)
{
    double maxx = 1.0, maxy = 1.0;
    double minx = 1.0, miny = 1.0;
    int iterations = 1000000;
    double x = 1.0, y = 1.0;    /* Initial values */
    int i;

    for (i=0; i<iterations; i++) {
      henon(&x, &y);
      if (x>maxx) maxx = x;
      if (x<minx) minx = x;
      if (y>maxy) maxy = y;
      if (y<miny) miny = y;
    }
    printf("x has range (%f,%f)\ny has range (%f,%f)\n",
            minx,maxx,miny,maxy);
}