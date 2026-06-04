/*
Mariana Ramirez
A01787819
 */

#include <iostream>
#include <vector>
#include <thread>

using namespace std;

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

double computePiSequential(long n) {
    double width = 1.0 / n;
    double sum = 0.0;

    for (long i = 0; i < n; i++) {
        double mid = (i + 0.5) * width;
        sum += f(mid);
    }

    return sum * width;
}

void computeRange(long start, long end, long n, double &result) {
    double width = 1.0 / n;
    result = 0.0;

    for (long i = start; i <= end; i++) {
        double mid = (i + 0.5) * width;
        result += f(mid);
    }
}

double computePiParallel(long n, int numThreads) {
    vector<thread> threads;
    vector<double> results(numThreads);

    long rangeSize = n / numThreads;
    long start = 0;

    for (int i = 0; i < numThreads; i++) {
        long end;

        if (i == numThreads - 1) {
            end = n - 1;
        } else {
            end = start + rangeSize - 1;
        }

        threads.push_back(thread(computeRange, start, end, n, ref(results[i])));

        start = end + 1;
    }

    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
    }

    double total = 0.0;

    for (int i = 0; i < numThreads; i++) {
        total += results[i];
    }

    return total * (1.0 / n);
}

int main() {
    long n = 100000000;
    int numThreads = thread::hardware_concurrency();

    double sequential = computePiSequential(n);
    double parallel = computePiParallel(n, numThreads);

    cout.precision(20);

    cout << "Sequential pi: " << sequential << endl;
    cout << "Parallel pi:   " << parallel << endl;
    cout << "Reference pi:  " << 3.14159265358979323846 << endl;

    return 0;
}