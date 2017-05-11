#include <stdio.h>

typedef struct comp
 {
   float real;
   float imag;
 } complex;

complex mult(complex a,complex b)
{
 complex c;
 c.real=a.real*b.real - a.imag*b.imag;
 c.imag=a.real*b.imag + a.imag*b.real;
 return c;
}

int main()
{

  float re,im;
  complex a,b;
    
   printf("Please enter the first complex number (re, im): ");
   scanf("%f%f", &re, &im);
   
   a.real = re; a.imag = im;

   printf("Please enter the second complex number (re, im): ");
   scanf("%f%f", &re, &im); 

   b.real = re; b.imag = im;

   a = mult(a,b);

   printf("Their product is %f + %fi \n", a.real, a.imag);
   return 0;
}
