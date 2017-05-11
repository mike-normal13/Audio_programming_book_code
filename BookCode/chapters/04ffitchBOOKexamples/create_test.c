#define SIZE (1024)
#define TWOPIBYSIZE (6.28318530718/(double)SIZE)
int main(void)
{
    int i;
    printf("%d\n", SIZE);
    for (i=0; i<SIZE; i++)
      printf("%f\n", sin((double)i*TWOPIBYSIZE));
}
