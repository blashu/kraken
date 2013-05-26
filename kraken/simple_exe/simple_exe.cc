#include <stdio.h>
#include <stdlib.h>

unsigned int fibonacci(int x)                 /* compute fibonacci number recursively */
{
  if (x > 2)
    return (fibonacci(x - 1) + fibonacci(x - 2));
  else
    return (1);
}

int main()
{
  int i, numtimes, number;
  unsigned int value, fib();

  printf("Input number of iterations: ");
  scanf ("%d", &numtimes);
  for (i = 1; i <= numtimes; i++)
  {
    printf ("Input number: ");
    scanf ("%d", &number);
    value = fibonacci(number);
    printf("fibonacci(%d) = %u\n", number, value);
  }
  exit(0);
}
