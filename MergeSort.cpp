#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <chrono>
#include <random>

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> left_half(arr.begin() + left, arr.begin() + left + n1);
    std::vector<int> right_half(arr.begin() + mid + 1, arr.begin() + mid + 1 + n2);

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

void merge_sort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void merge_sort_parallel(std::vector<int>& arr, int left, int right, int depth) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if (depth > 0) {
            std::thread left_thread(merge_sort_parallel, std::ref(arr), left, mid, depth - 1);
            std::thread right_thread(merge_sort_parallel, std::ref(arr), mid + 1, right, depth - 1);

            left_thread.join();
            right_thread.join();
        } else {
            merge_sort(arr, left, mid);
            merge_sort(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

int main() {
    std::vector<int> arr;
    int n = 10000; // Adjust the size of the array

    // Generate a random worst-case scenario array
    for (int i = n; i > 0; i--) {
        arr.push_back(i);
    }

    // Measure time for standard merge sort
    auto start = std::chrono::high_resolution_clock::now();
    merge_sort(arr, 0, n - 1);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Standard Merge Sort Time: " << elapsed.count() << " seconds" << std::endl;

    // Generate a random best-case scenario array
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(arr.begin(), arr.end(), g);

    // Measure time for multithreaded merge sort
    start = std::chrono::high_resolution_clock::now();
    int depth = 3; // Adjust the depth for parallelism
    merge_sort_parallel(arr, 0, n - 1, depth);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Multithreaded Merge Sort Time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
