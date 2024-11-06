#include <iostream>
#include "../Task 3/sorts.cpp"
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

const int TIMEOUT_LIMIT = 7; // Timeout limit in seconds

void test(vector<long> entries)
{
    using namespace std::chrono;
    auto timeStart = high_resolution_clock::now();

    vector<long> result = MergeSort(entries);
    auto elapsed = duration_cast<seconds>(high_resolution_clock::now() - timeStart).count();

    // Check if sorting took too long
    if (elapsed > TIMEOUT_LIMIT) {
        cout << "Timed Out Test Failed" << endl;
        return;
    }

    // Check if the result is sorted correctly
    sort(entries.begin(), entries.end());
    for (size_t i = 0; i < entries.size(); i++) {
        if (entries[i] != result[i]) {
            cout << "OUTPUT NOT SORTED TEST FAILED EXITING!" << endl;
            return;
        }
    }

    auto timeEnd = high_resolution_clock::now();
    duration<double> totalTime = duration_cast<duration<double>>(timeEnd - timeStart);
    cout << "TEST PASSED IN : " << totalTime.count() << " SECONDS." << endl;
}

int main()
{
    vector<long> entries;
    srand(time(NULL));

    for (int i = 0; i < 10000; i++) {
        entries.push_back(-12500 + rand() % 25000);
    }

    test(entries);
    return 0;
}