/*
Mariana Ramirez
A01787819
 */

#include <iostream>
#include <vector>
#include <thread>
#include <cmath>

using namespace std;

bool isPrime(long n) {
    if (n < 2) {
        return false;
    }

    if (n == 2) {
        return true;
    }

    for (long i = 2; i <= ceil(sqrt(n)); i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

unsigned long long sumPrimesSequential(long n) {
    unsigned long long sum = 0;

    for (long i = 2; i <= n; i++) {
        if (isPrime(i)) {
            sum += i;
        }
    }

    return sum;
}

void sumRange(long start, long end, unsigned long long &result) {
    result = 0;

    for (long i = start; i <= end; i++) {
        if (isPrime(i)) {
            result += i;
        }
    }
}

unsigned long long sumPrimesParallel(long n, int numThreads) {
    vector<thread> threads;
    vector<unsigned long long> results(numThreads);

    long rangeSize = n / numThreads;
    long start = 2;

    for (int i = 0; i < numThreads; i++) {
        long end;

        if (i == numThreads - 1) {
            end = n;
        } else {
            end = start + rangeSize - 1;
        }

        threads.push_back(thread(sumRange, start, end, ref(results[i])));

        start = end + 1;
    }

    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
    }

    unsigned long long total = 0;

    for (int i = 0; i < numThreads; i++) {
        total += results[i];
    }

    return total;
}

int main() {
    long n = 1000000;
    int numThreads = thread::hardware_concurrency();

    unsigned long long sequential = sumPrimesSequential(n);
    unsigned long long parallel = sumPrimesParallel(n, numThreads);

    cout << "Sequential result: " << sequential << endl;
    cout << "Parallel result:   " << parallel << endl;

    if (sequential == parallel) {
        cout << "The results are the same." << endl;
    } else {
        cout << "The results are different." << endl;
    }

    return 0;
}