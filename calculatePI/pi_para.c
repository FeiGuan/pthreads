#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
 
#define NUM_THREADS 4
#define N 100

pthread_mutex_t pmutex;
double sum = 0.0;

void *slot (void *t)
{
	int tNum = *((int *) t);
	int start, end;
	int i;

	start = (N/NUM_THREADS) * tNum;
	end = (N/NUM_THREADS) * (tNum + 1);
	if (tNum == (NUM_THREADS - 1))
		end = N;

	double factor = (start % 2 == 0 ? 1.0 : -1.0);
	for (i = start; i < end; i++)
	{
		pthread_mutex_lock(&pmutex);
		sum += factor / (2*i + 1);
		factor = -factor;
		pthread_mutex_unlock(&pmutex);
	}
}

double calculate()
{
	int i;
	pthread_t tHandlers[NUM_THREADS];
	pthread_mutex_init(&pmutex, NULL);
	for (i = 0; i < NUM_THREADS; i++)
	{
		int *threadNum = (int*)malloc(sizeof(int));
		*threadNum = i;
		pthread_create(&tHandlers[i], NULL, slot, (void*)(threadNum));
	}

	for (i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(tHandlers[i], NULL);
	}

	pthread_mutex_destroy(&pmutex);
	return 4.0 * sum;
}

int main()
{
	double pi = calculate();
	printf("pi = %f\n", pi);
}