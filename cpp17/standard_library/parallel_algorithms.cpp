// parallel_algorithms.cpp
#include <iostream>
#include <vector>
#include <algorithm>  // For std::sort, std::for_each, std::transform, std::reduce
#include <execution>  // Key header for parallel execution policies (C++17)
#include <numeric>    // For std::iota, std::reduce (though reduce is also in <algorithm> in C++17)
#include <chrono>     // For timing
#include <iomanip>    // For std::fixed, std::setprecision

// Helper function to print a vector
template<typename T>
void print_vector(const std::string& title, const std::vector<T>& v, size_t limit = 10) {
    std::cout << title << " (first " << std::min(limit, v.size()) << " of " << v.size() << " elements): [";
    for (size_t i = 0; i < v.size() && i < limit; ++i) {
        std::cout << v[i] << (i == v.size() - 1 || i == limit - 1 ? "" : ", ");
    }
    if (v.size() > limit) std::cout << ", ...";
    std::cout << "]" << std::endl;
}

// A somewhat CPU-intensive operation for demonstration
long long complex_calculation(int val) {
    long long res = 0;
    for (int i = 0; i < 500; ++i) { // Loop to make it take some time
        res += static_cast<long long>(val) * val * i / (i + 1);
        res %= 1000000007; // Keep it within bounds
    }
    return res;
}

int main() {
    std::cout << "--- Parallel Algorithms (C++17) ---" << std::endl;
    const size_t data_size = 2000000; // 2 million elements
    std::vector<int> v_orig(data_size);
    std::iota(v_orig.begin(), v_orig.end(), 1); // Fill with 1, 2, ..., data_size

    std::vector<int> v_seq, v_par, v_par_unseq;
    std::vector<long long> v_transformed_seq(data_size), v_transformed_par(data_size);

    // --- 1. std::sort ---
    std::cout << "\n--- 1. std::sort ---" << std::endl;
    // Make copies for sorting
    v_seq = v_orig;
    std::reverse(v_seq.begin(), v_seq.end()); // Make it unsorted (worst case for some sorts)
    v_par = v_seq;
    v_par_unseq = v_seq;

    auto start_time = std::chrono::high_resolution_clock::now();
    std::sort(std::execution::seq, v_seq.begin(), v_seq.end());
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_seq_sort = end_time - start_time;
    std::cout << "std::sort (seq) time: " << std::fixed << std::setprecision(2) << time_seq_sort.count() << " ms" << std::endl;
    print_vector("Sorted v_seq", v_seq);

    start_time = std::chrono::high_resolution_clock::now();
    std::sort(std::execution::par, v_par.begin(), v_par.end());
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_par_sort = end_time - start_time;
    std::cout << "std::sort (par) time: " << time_par_sort.count() << " ms" << std::endl;
    print_vector("Sorted v_par", v_par);
    // Check correctness (optional)
    if(v_seq != v_par) std::cout << "ERROR: par sort differs from seq sort!" << std::endl;


    start_time = std::chrono::high_resolution_clock::now();
    std::sort(std::execution::par_unseq, v_par_unseq.begin(), v_par_unseq.end());
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_par_unseq_sort = end_time - start_time;
    std::cout << "std::sort (par_unseq) time: " << time_par_unseq_sort.count() << " ms" << std::endl;
    print_vector("Sorted v_par_unseq", v_par_unseq);
     if(v_seq != v_par_unseq) std::cout << "ERROR: par_unseq sort differs from seq sort!" << std::endl;


    // --- 2. std::for_each ---
    // Note: Operations in parallel for_each should be independent and thread-safe.
    // Modifying a shared variable without synchronization is unsafe.
    // Here, we'll use it to call a function that has side effects (printing)
    // but this is generally discouraged for parallel execution if order matters or if side effects are not thread-safe.
    std::cout << "\n--- 2. std::for_each (demonstrating usage, not ideal for parallel cout) ---" << std::endl;
    std::vector<int> short_vec = {1, 2, 3, 4, 5, 6, 7, 8};
    std::cout << "std::for_each (seq): ";
    std::for_each(std::execution::seq, short_vec.begin(), short_vec.end(), [](int x){ std::cout << x*x << " "; });
    std::cout << std::endl;

    std::cout << "std::for_each (par): "; // Output order might be jumbled
    std::for_each(std::execution::par, short_vec.begin(), short_vec.end(), [](int x){ std::cout << x*x << " "; });
    std::cout << std::endl;


    // --- 3. std::transform ---
    std::cout << "\n--- 3. std::transform (with complex_calculation) ---" << std::endl;
    v_orig.resize(100000); // Reduce size for transform to run faster in demo

    start_time = std::chrono::high_resolution_clock::now();
    std::transform(std::execution::seq, v_orig.begin(), v_orig.end(), v_transformed_seq.begin(), complex_calculation);
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_seq_transform = end_time - start_time;
    std::cout << "std::transform (seq) time: " << time_seq_transform.count() << " ms" << std::endl;
    print_vector("Transformed v_seq", v_transformed_seq);

    start_time = std::chrono::high_resolution_clock::now();
    std::transform(std::execution::par, v_orig.begin(), v_orig.end(), v_transformed_par.begin(), complex_calculation);
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_par_transform = end_time - start_time;
    std::cout << "std::transform (par) time: " << time_par_transform.count() << " ms" << std::endl;
    print_vector("Transformed v_par", v_transformed_par);
    // Check correctness (optional)
    // if(v_transformed_seq != v_transformed_par) std::cout << "ERROR: par transform differs from seq!" << std::endl;


    // --- 4. std::reduce (parallel sum) ---
    // std::reduce is like std::accumulate but can be parallelized.
    // It requires the operation to be associative and commutative for effective parallelization.
    std::cout << "\n--- 4. std::reduce ---" << std::endl;
    v_orig.resize(data_size); // Restore original size for reduce
    std::iota(v_orig.begin(), v_orig.end(), 1);

    long long sum_seq, sum_par, sum_par_unseq;

    start_time = std::chrono::high_resolution_clock::now();
    sum_seq = std::reduce(std::execution::seq, v_orig.begin(), v_orig.end(), 0LL); // 0LL is initial sum
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_seq_reduce = end_time - start_time;
    std::cout << "std::reduce (seq) sum: " << sum_seq << ", time: " << time_seq_reduce.count() << " ms" << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    sum_par = std::reduce(std::execution::par, v_orig.begin(), v_orig.end(), 0LL);
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_par_reduce = end_time - start_time;
    std::cout << "std::reduce (par) sum: " << sum_par << ", time: " << time_par_reduce.count() << " ms" << std::endl;
    if(sum_seq != sum_par) std::cout << "ERROR: par reduce sum differs from seq!" << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    // The following line was an example of reduce with transform, but for fair timing, we use simple reduce.
    // sum_par_unseq = std::reduce(std::execution::par_unseq, v_orig.begin(), v_orig.end(), 0LL, std::plus<>(), [](int val){ return static_cast<long long>(val*2); });
    sum_par_unseq = std::reduce(std::execution::par_unseq, v_orig.begin(), v_orig.end(), 0LL);
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_par_unseq_reduce = end_time - start_time;
    std::cout << "std::reduce (par_unseq) sum: " << sum_par_unseq << ", time: " << time_par_unseq_reduce.count() << " ms" << std::endl;
    if(sum_seq != sum_par_unseq) std::cout << "ERROR: par_unseq reduce sum differs from seq!" << std::endl;


    std::cout << "\nNote: Actual performance gains from parallel policies depend heavily on the hardware," << std::endl;
    std::cout << "the nature of the operation, data size, and the quality of the standard library implementation." << std::endl;
    std::cout << "Overhead of parallelization can make it slower for small datasets or trivial operations." << std::endl;

    return 0;
}

/*
Explanation:
C++17 introduced support for parallel versions of many standard library algorithms.
This allows developers to request that an algorithm be executed in parallel,
potentially taking advantage of multi-core processors to speed up computations.

Key Header: `<execution>`
This header defines the execution policies.

Execution Policies:
These are special tags passed as the first argument to parallelizable algorithms
to specify how they should be executed:

1.  `std::execution::sequenced_policy` (tag: `std::execution::seq`)
    -   Specifies that the algorithm must be executed sequentially on the calling thread.
    -   No parallelism is allowed. This is the default behavior if no policy is specified.

2.  `std::execution::parallel_policy` (tag: `std::execution::par`)
    -   Specifies that the algorithm may be executed in parallel.
    -   The execution may be divided into multiple threads.
    -   If an element access function (like a lambda passed to `for_each` or `transform`)
        throws an exception, `std::terminate` is called.
    -   Order of operations may be indeterministic for some algorithms if not carefully managed.
        Synchronization is required if accessing shared data that is not protected.

3.  `std::execution::parallel_unsequenced_policy` (tag: `std::execution::par_unseq`)
    -   Specifies that the algorithm may be executed in parallel and may also use
        vectorized instructions (SIMD - Single Instruction, Multiple Data).
    -   This is the most permissive policy, allowing the most optimization freedom
        to the implementation.
    -   Imposes stricter constraints:
        -   Element access functions must not call any functions that could lead to
          deadlock if called concurrently from different threads or from vectorized code
          (e.g., standard library functions that might acquire locks, memory allocation/deallocation).
        -   No synchronization is allowed within the element access functions that could cause deadlock.
        -   Exceptions behave as with `std::execution::par`.

Which Algorithms are Parallelizable?
A significant number of STL algorithms were overloaded to accept execution policies, including:
- `std::sort`, `std::stable_sort`
- `std::for_each`, `std::for_each_n`
- `std::transform`
- `std::reduce` (new in C++17, similar to `std::accumulate` but parallel-friendly)
- `std::accumulate` (parallel version also added)
- `std::count`, `std::count_if`
- `std::find`, `std::find_if`, `std::find_if_not`
- `std::all_of`, `std::any_of`, `std::none_of`
- `std::copy`, `std::move`, `std::fill`, `std::generate`
- And many more.

Usage Example:
```cpp
std::vector<int> data = { ... };
// Sort in parallel
std::sort(std::execution::par, data.begin(), data.end());

// Transform in parallel
std::vector<int> results(data.size());
std::transform(std::execution::par, data.begin(), data.end(), results.begin(),
               [](int x){ return x * x; });
```

Considerations:
-   Overhead: Parallel execution has overhead (thread creation, synchronization, task division).
    For small datasets or very fast operations, parallel versions might be slower than sequential ones.
-   Data Races: If the operations performed by the algorithm on different elements are not
    independent (e.g., they modify shared data without proper synchronization), data races
    and undefined behavior can occur. The user is responsible for ensuring thread safety.
-   Exceptions: If an exception is thrown by an element access function invoked as part of a
    parallel algorithm (with `par` or `par_unseq`), `std::terminate` is called, ending the program.
    This means error handling within the parallelized operations needs to be carefully considered.
-   Implementation Quality: The actual performance benefits depend on the standard library's
    implementation of parallel algorithms and the underlying hardware.
-   Debugging: Debugging parallel code can be more challenging.

`std::reduce`:
-   `std::reduce` was introduced in C++17 as a parallel-friendly version of `std::accumulate`.
-   It computes a sum (or other binary operation) over a range.
-   Unlike `std::accumulate`, `std::reduce` allows the binary operation to be applied in an
    unspecified order and out of sequence, making it suitable for parallel execution.
    The operation should ideally be associative and commutative for best results.

How to compile:
-   g++: `g++ -std=c++17 parallel_algorithms.cpp -o parallel_algorithms_example -pthread -TBB` (or other threading library like OpenMP if the libstdc++ is configured for it)
    Often, just `-pthread` is enough if the default libstdc++ supports it. Some implementations might require linking against Intel TBB (`-ltbb`).
-   Clang: `clang++ -std=c++17 parallel_algorithms.cpp -o parallel_algorithms_example -pthread` (similar to g++, may depend on libc++ configuration)
-   MSVC: `/std:c++17 /EHsc` (Parallel algorithms are generally supported).
    Check your compiler/library documentation for specific flags for parallel algorithm support.
*/
