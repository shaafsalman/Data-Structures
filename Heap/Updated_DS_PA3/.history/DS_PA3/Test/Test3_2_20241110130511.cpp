#include <iostream>
#include "../Task 3/sorts.cpp"
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

// Timeout limit in seconds
const int TIMEOUT_LIMIT = 7;

void test(vector<long> entries)
{
    using namespace std::chrono;
    auto timeStart = high_resolution_clock::now();

    // Perform the sorting
    vector<long> result = InsertionSort(entries);

    // Check if the sorting exceeded the timeout
    auto elapsed = duration_cast<seconds>(high_resolution_clock::now() - timeStart).count();
    if (elapsed > TIMEOUT_LIMIT) {
        cout << "Timed Out Test Failed" << endl;
        return;
    }
      cout << "First few elements (result): ";
    for (int i = 0; i < 5 && i < result.size(); i++) {
        cout << result[i] << " ";
    }
    cout << "\nLast few elements (result): ";
    for (int i = result.size() - 5; i < result.size(); i++) {
        cout << result[i] << " ";
    }
    cout << endl;

    // Verify if the result is sorted correctly
    sort(entries.begin(), entries.end());
    for (size_t i = 0; i < entries.size(); i++) {
        if (entries[i] != result[i]) {
            cout << "OUTPUT NOT SORTED TEST FAILED EXITING!" << endl;
            return;
        }
    }

    // If sorting and verification passed within time
    auto timeEnd = high_resolution_clock::now();
    duration<double> totalTime = duration_cast<duration<double>>(timeEnd - timeStart);
    cout << "TEST PASSED IN : " << totalTime.count() << " SECONDS." << endl;
}

int main()
{
    vector<long> entries;
    srand(time(NULL));

    // Generate random test data
    for (int i = 0; i < 75000; i++) {
        entries.push_back(-75000 + rand() % 150000);
    }

    // Run the test
    test(entries);
    return 0;
}