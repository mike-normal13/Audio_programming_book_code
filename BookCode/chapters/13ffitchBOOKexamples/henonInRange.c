#include <stdio.h>

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
    int iterations = 1000;
    int count[13];
    double x = 1.0, y = 1.0;    /* Initial values */
    int i;

    for (i=0; i<13; i++) count[i] = 0;

    for (i=0; i<iterations; i++) {
      int instr;
      henon(&x, &y);
      instr = (int)(1.0+(x+0.860850)*5.841409);
      count[instr-1]++;
      printf("i%d %d\n", instr, i);
    }
    printf("\nSTATICS:\n");
    for (i=0; i<13; i++) printf("%d ",count[i]);
    printf("\n");
}
