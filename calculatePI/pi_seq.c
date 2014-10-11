#include <stdio.h>

#define N 10000

int main()
{
	int k;
	double factor = 1.0;
	double sum = 0.0;
	for (k = 0; k < N; k++) {
		sum += factor / (2*k + 1);
		factor = -factor;
	}
	double pi_approx = 4.0 * sum;
	printf("%f\n", pi_approx);
}