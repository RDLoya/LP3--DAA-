#include <vector>
#include <thread>
#include <chrono>   // Include the chrono library for time measurements
using namespace std;

// Function to merge two sorted subarrays
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> left_half(n1);
    vector<int> right_half(n2);

    for (int i = 0; i < n1; i++)
        left_half[i] = arr[left + i];
    for (int i = 0; i < n2; i++)
        right_half[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (left_half[i] <= right_half[j]) {
            arr[k] = left_half[i];
            i++;
        } else {
            arr[k] = right_half[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = left_half[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = right_half[j];
        j++;
        k++;
    }
}

// Merge Sort algorithm
void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// Multithreaded merge sort
void multiThreadedMergeSort(vector<int>& arr, int left, int right, int threads) {
    if (threads <= 1 || (right - left + 1) <= 1000) {
        mergeSort(arr, left, right);
    } else {
        int mid = left + (right - left) / 2;

        thread leftThread(multiThreadedMergeSort, ref(arr), left, mid, threads / 2);
        thread rightThread(multiThreadedMergeSort, ref(arr), mid + 1, right, threads / 2);

        leftThread.join();
        rightThread.join();

        merge(arr, left, mid, right);
    }
}

int main() {
    vector<int> arr;
    int size, threads;

    // Input array size and number of threads
    cout << "Enter the size of the array: ";
    cin >> size;

    cout << "Enter the number of threads: ";
    cin >> threads;

    // Input array elements
    cout << "Enter the array elements:" << endl;
    for (int i = 0; i < size; i++) {
        int element;
        cin >> element;
        arr.push_back(element);
    }

    // Perform multithreaded merge sort
    multiThreadedMergeSort(arr, 0, size - 1, threads);

    // Print the sorted array
    cout << "Sorted array: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
     // Measure time for merge sort
    auto start_time_merge = chrono::high_resolution_clock::now();
    mergeSort(arr, 0, size - 1);
    auto end_time_merge = chrono::high_resolution_clock::now();
    chrono::duration<double> mergeSortTime = end_time_merge - start_time_merge;

    // Measure time for multithreaded merge sort
    auto start_time_multiThreaded = chrono::high_resolution_clock::now();
    multiThreadedMergeSort(arr, 0, size - 1, threads);
    auto end_time_multiThreaded = chrono::high_resolution_clock::now();
    chrono::duration<double> multiThreadedMergeSortTime = end_time_multiThreaded - start_time_multiThreaded;

    // Print the time taken for each sorting method
    cout << "Merge Sort Time: " << mergeSortTime.count() << " seconds" << endl;
    cout << "Multithreaded Merge Sort Time: " << multiThreadedMergeSortTime.count() << " seconds" << endl;
    return 0;
}
