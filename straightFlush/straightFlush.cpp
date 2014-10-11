#include<pthread.h>
#include<iostream>
#include<unordered_set>
#include<algorithm>
#include<sys/time.h>
#include<unistd.h>

using namespace std;


/*
 This program simulates the cards in the hand for 400,000 times. For each
 time, we use a set to record 17 random numbers representing the cards.
 Then we calculate the maximum length of straight flush. After 400,000 times
 of simulation, the sum of straight flush lengths is acquired. The approximate
 length can be calculated as the sum divided by 400,000.

 Output: 
 Straight Flush: [the approximate lenth of straight flush]
 Time Used: [the simulation time in milliseconds]ms
*/

/*
 simOnce: simulate a single case.
*/
int simOnce ()
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
	return maxFlush;
}

/*
 simulate n times
*/
double simulate(int n) {
	long sum = 0;
	for (int i = 0; i < n; i++) {
		sum += simOnce();
	}
	return (double)sum / (double)n;
}

int main() {
	struct timeval start, end;
    long mtime, seconds, useconds;    

    gettimeofday(&start, NULL);
    double final = simulate(400000);
    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	
	cout << "Straight Flush: " << final << endl;
    cout << "Time Used: " << mtime << "ms" << endl;
}