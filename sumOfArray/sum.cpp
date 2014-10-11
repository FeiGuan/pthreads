#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define NUM_THREADS 4

pthread_mutex_t pmutex;
int N;
int *X;
int gSum[NUM_THREADS];

void *Summation (void* pArg)
{
	int tNum = *((int *) pArg);
	int lSum = 0;
	int start, end;
	
	start = (N/NUM_THREADS) * tNum;
	end = (N/NUM_THREADS) * (tNum + 1);
	if (tNum == (NUM_THREADS - 1))
		end = N;

	for (int i = start; i < end; i++) {
		lSum += X[i];
	}

	pthread_mutex_lock(&pmutex);
	gSum[tNum] = lSum;
	pthread_mutex_unlock(&pmutex);
	free(pArg);
}

int main(int argc, char* argv[])
{
	int j, sum = 0;
	pthread_t tHandlers[NUM_THREADS];
	pthread_mutex_init(&pmutex, NULL);

	N = 10000;
	X = new int[N];
	for (j=0; j < N; j++) {
		X[j] = j;
	}
	for (j=0; j < NUM_THREADS; j++) {
		int *threadNum = new(int);
		*threadNum = j;
		pthread_create(&tHandlers[j], NULL, Summation, (void*)threadNum);
	}
	for (j = 0; j < NUM_THREADS; j++) {
		pthread_join(tHandlers[j], NULL);
		sum += gSum[j];
	}
	pthread_mutex_destroy(&pmutex);
	printf("The sum of array elements is %d\n", sum);
	return 0;
}