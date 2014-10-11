#include<pthread.h>
#include<iostream>
#include<unordered_set>
#include<algorithm>
#include<pthread.h>
#include<sys/time.h>
#include<unistd.h>

using namespace std;

#define NUM_THREADS 2
#define TIMES 200000

/*
 This program simulates the cards in the hand for 400,000 times. For each
 time, we use a set to record 17 random numbers representing the cards.
 Then we calculate the maximum length of straight flush. After 400,000 times
 of simulation, the sum of straight flush lengths is acquired. The approximate
 length can be calculated as the sum divided by 400,000.

 In the set, the 17 numbers are assigned with different numbers between 0 and 
 51(inclusive). 0~12, 13~25, 26~38, 39~51 represent the cards of each suit.

 Multi-threading is used in this implementation. NUM_THREADS is the number of
 threads used (here we set it as 2). TIMES is the number of times each thread
 will simulate.

 Output: 
 Straight Flush: [the approximate lenth of straight flush]
 Time Used: [the simulation time in milliseconds]ms
*/


pthread_mutex_t pmutex;
int sum = 0;

/*
 simTimes: routine of each thread. 
*/
void *simNTimes (void *t)
{
	for (int k = 0; k < TIMES; k++)
	{
		unordered_set<int> nums;
		// add 17 random numbers to the set
		while (nums.size() < 17)
		{
			int num = rand() % 52;
			unordered_set<int>::const_iterator got = nums.find(num);
			if (got == nums.end())
				nums.insert(num);
		}
		int maxFlush = 0;
		// each loop iterates over each suit
		for (int j = 0; j < 4; j++) {
			bool consecutive = false;
			int temp = 0;
			// find consecutive numbers
			for (int i = 13*j; i < 13*j + 13; i++) {
				if (consecutive) {
					unordered_set<int>::const_iterator got = nums.find(i);
					if (got != nums.end()) {
						temp++;
					} else {
						maxFlush = max(maxFlush, temp);
						temp = 0;
						consecutive = false;
					}
				} else {
					unordered_set<int>::const_iterator got = nums.find(i);
					if (got != nums.end()) {
						temp++;
						consecutive = true;
					} else {
						continue;
					}
				}
			}
			if (consecutive)
				maxFlush = max(maxFlush, temp);
		}
		// the mutex only allows one thread at a time to modify the sum
		pthread_mutex_lock(&pmutex);
		sum += maxFlush;
		pthread_mutex_unlock(&pmutex);
	}
	pthread_exit(0);
}
	
double simulate() {
	pthread_t threads[NUM_THREADS];
	pthread_mutex_init(&pmutex, NULL);
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&threads[i], NULL, simNTimes, NULL);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&pmutex);
	return (double)sum / (double)(TIMES * NUM_THREADS);
}
	
int main() {
	struct timeval start, end;
    long mtime, seconds, useconds;    

    gettimeofday(&start, NULL);
    double final = simulate();
    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	
	cout << "Straight Flush: " << final << endl;
    cout << "Time Used: " << mtime << "ms" << endl;
}