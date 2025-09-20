// COMSC-210 | Lab 12 (STD::array) | Chevin Jeon 
// IDE used: C++11 VS Code
//
// Real-world simulation: Daily step counts for a 30-day fitness tracker.
// - Reads 30 integers from an external file (one per line) into std::array<int, 30>.
// - Demonstrates std::array member functions and iterator-based operations.
// - Validates file open and data length, prints specific error messages, and shows results.

#include <iostream>     // cout, cin
#include <fstream>      // ifstream
#include <array>        // std::array
#include <algorithm>    // min_element, max_element, sort, copy
#include <numeric>      // accumulate
#include <iomanip>      // setw
#include <string>       // string
using namespace std;

//======================== Constants ========================

const string INPUT_FILE = "steps.txt";   // external data file (30 integers, one per line)
const size_t DAYS = 30;                  // required number of elements for the lab
const int FIELD_W = 6;                   // setw width for printing


//==================== Function Prototypes ==================

// loadSteps() reads exactly 30 integers from a file into an std::array
// arguments: const string& filename, array<int, DAYS>& out
// returns: bool (true if success, false otherwise)
bool loadSteps(const string& filename, array<int, DAYS>& steps);


// printArray() prints the array in rows for readability
// arguments: const array<int, DAYS>& steps
// returns: nothing
void printArray(const array<int, DAYS>& steps);

// computeAndPrintStats() shows min, max, sum, average and front/back via member functions
// arguments: const array<int, DAYS>& steps
// returns: nothing
void computeAndPrintStats(const array<int, DAYS>& steps);

// mutateWithDataPointer() demonstrates array::data() by incrementing all elements via raw pointer
// arguments: array<int, DAYS>& steps, int delta
// returns: nothing
void mutateWithDataPointer(array<int, DAYS>& steps, int delta);

// demoArrayMembers() exercises various std::array members: size(), max_size(), at(), front(), back(), fill(), swap()
// arguments: array<int, DAYS>& steps
// returns: nothing
void demoArrayMembers(array<int, DAYS>& steps);


// printSortedTopK() copies to a temp array, sorts it, and prints top K values
// arguments: const array<int, DAYS>& steps, size_t k
// returns: nothing
void printSortedTopK(const array<int, DAYS>& steps, size_t k);

//======================== main() ===========================

int main() {
    // Program header output
    cout << "COMSC-210 | Lab 12 (STD::array) | Your Name\n";
    cout << "Reading 30 daily step counts from file and exercising std::array...\n\n";

    // Declare the std::array. Default-initialized values are unspecified; we'll read into it.
    array<int, DAYS> steps;

    // Load data from file with validation and specific error messages.
    if (!loadSteps(INPUT_FILE, steps)) {
        cout << "Terminating program due to input error.\n";
        return 1;
    }

    // Show the raw data
    cout << "Raw step counts (from file):\n";
    printArray(steps);

    // Demonstrate computations using iterators and member functions
    computeAndPrintStats(steps);

    // Demonstrate various std::array members in action
    demoArrayMembers(steps);

    // Show effect of demo mutations
    cout << "\nAfter demoArrayMembers() mutations:\n";
    printArray(steps);

    // Demonstrate data() pointer-based mutation (increment all days by +100)
    mutateWithDataPointer(steps, /*delta=*/100);
    cout << "\nAfter mutateWithDataPointer(+100) via array::data():\n";
    printArray(steps);

    // Print Top-5 days after sorting a copy (original remains in original order)
    printSortedTopK(steps, 5);

    cout << "\nDone. Program completed successfully.\n";
    return 0;
}


//================= Function Definitions ====================

// loadSteps() reads exactly DAYS integers from filename into 'steps'.
// Validates: file open, numeric extraction, exact count.
bool loadSteps(const string& filename, array<int, DAYS>& steps) {
    ifstream in(filename.c_str());
    if (!in) {
        cout << "ERROR: Could not open input file '" << filename << "'. "
             << "Please ensure the file exists in the working directory.\n";
        return false;
    }

    size_t count = 0;
    // Use operator[] for speed; we also show at() later in demo functions
    while (count < DAYS && (in >> steps[count])) {
        ++count;
    }

    // If extraction failed mid-way, report a specific error (non-integer or premature EOF)
    if (count < DAYS) {
        cout << "ERROR: Only read " << count << " values from '" << filename
             << "'. Expected exactly " << DAYS << " integers (one per line).\n"
             << "Tip: Check for non-numeric characters or missing lines.\n";
        return false;
    }

    // If file contains extra tokens beyond DAYS, that’s fine for this lab—but we can warn:
    int dummy;
    if (in >> dummy) {
        cout << "WARNING: File contains more than " << DAYS
             << " integers. Only the first " << DAYS << " were used.\n";
    }

    return true;
}

// printArray() pretty-prints the array values in 10 columns per line.
void printArray(const array<int, DAYS>& steps) {
    const int COLS = 10; // constant local formatting
    for (size_t i = 0; i < steps.size(); ++i) {
        cout << setw(FIELD_W) << steps[i];
        if ((i + 1) % COLS == 0) cout << '\n';
    }
    if (DAYS % COLS != 0) cout << '\n';
}

// computeAndPrintStats() uses iterators and member accessors to summarize the data.
void computeAndPrintStats(const array<int, DAYS>& steps) {
    // Sum and average using <numeric>
    long long sum = accumulate(steps.begin(), steps.end(), 0LL);
    double avg = static_cast<double>(sum) / static_cast<double>(steps.size());

    // Min/Max using <algorithm>
    auto itMin = min_element(steps.begin(), steps.end());
    auto itMax = max_element(steps.begin(), steps.end());

    cout << "\n--- Stats ---\n";
    cout << "Days (size): " << steps.size() << " (max_size: " << steps.max_size() << ")\n";
    cout << "Front (day 1): " << steps.front() << "\n";
    cout << "Back  (day " << steps.size() << "): " << steps.back() << "\n";
    cout << "Sum: " << sum << "\n";
    cout << "Avg: " << fixed << setprecision(1) << avg << "\n";
    cout << "Min: " << *itMin << " (index " << (itMin - steps.begin()) << ")\n";
    cout << "Max: " << *itMax << " (index " << (itMax - steps.begin()) << ")\n";

    // Safe access demo with at(): show days 1..3 (bounds-checked)
    cout << "Preview via at(): ";
    for (size_t i = 0; i < 3 && i < steps.size(); ++i) {
        cout << steps.at(i) << (i + 1 < 3 ? ", " : "\n");
    }
}

// mutateWithDataPointer() uses array::data() to mutate underlying C-array.
void mutateWithDataPointer(array<int, DAYS>& steps, int delta) {
    int* p = steps.data(); // raw pointer to first element
    for (size_t i = 0; i < steps.size(); ++i) {
        p[i] += delta;     // increment each day by delta
    }
}

// demoArrayMembers() showcases: fill(), swap(), front(), back(), at(), size(), max_size().
// It prepares a second array and swaps to demonstrate ownership transfer.
void demoArrayMembers(array<int, DAYS>& steps) {
    // Make a baseline array filled with a constant (e.g., 5000 steps)
    array<int, DAYS> baseline;
    baseline.fill(5000); // set all elements to 5000 using array::fill()

    // Demonstrate swap(): exchange contents of steps and baseline
    cout << "\nSwapping with a baseline array (all 5000s) using array::swap()...\n";
    steps.swap(baseline);

    cout << "After swap, 'steps' should now be all 5000s (show first 10):\n";
    for (size_t i = 0; i < 10; ++i) {
        cout << setw(FIELD_W) << steps[i];
    }
    cout << "\n";

    // Put the original data back by swapping again
    cout << "Swapping back to restore original data...\n";
    steps.swap(baseline);

    // Use at() to safely bump the first and last days (bounds-checked)
    if (steps.size() >= 1) {
        steps.at(0) += 250; // day 1 bonus
        steps.at(steps.size() - 1) += 250; // last day bonus
    }

    // Show front()/back() after mutation
    cout << "front() after +250: " << steps.front() << ", back() after +250: " << steps.back() << "\n";
}

// printSortedTopK() copies to a temp array, sorts descending, and prints top-K values.
void printSortedTopK(const array<int, DAYS>& steps, size_t k) {
    if (k == 0) return;
    if (k > steps.size()) k = steps.size(); // input validation

    array<int, DAYS> copy = steps;                 // copy via aggregate assignment
    sort(copy.begin(), copy.end());                // ascending
    reverse(copy.begin(), copy.end());             // now descending

    cout << "\nTop-" << k << " days (sorted, descending):\n";
    for (size_t i = 0; i < k; ++i) {
        cout << setw(FIELD_W) << copy[i];
    }
    cout << "\n";
}