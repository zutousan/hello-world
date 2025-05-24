// std_latch.cpp
#include <iostream>
#include <latch>    // Key header for std::latch (C++20)
#include <thread>
#include <vector>
#include <string>
#include <chrono>   // For std::chrono::milliseconds
#include <syncstream> // For std::osyncstream (C++20) for synchronized cout

void worker_task(int id, std::latch& completion_latch, int work_duration_ms) {
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::milliseconds(work_duration_ms));
    
    // Use osyncstream for thread-safe output to std::cout
    std::osyncstream(std::cout) << "Worker " << id << " completed work after "
                                << work_duration_ms << "ms. Decrementing latch." << std::endl;
    
    // Decrement the latch counter.
    // If this is the last thread to decrement, threads waiting on the latch will be unblocked.
    completion_latch.count_down(); 
    // completion_latch.arrive_and_wait(); // Alternative: decrement and then wait for others
}

void dependent_task(int id, std::latch& start_latch) {
    std::osyncstream(std::cout) << "Dependent task " << id << " is waiting on the latch..." << std::endl;
    
    // Wait until the latch counter reaches zero.
    start_latch.wait();
    
    std::osyncstream(std::cout) << "Dependent task " << id << " unblocked and proceeding." << std::endl;
    // Simulate some follow-up work
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::osyncstream(std::cout) << "Dependent task " << id << " finished." << std::endl;
}

int main() {
    std::cout << "--- std::latch (C++20) ---" << std::endl;

    const int num_workers = 5;
    const int num_dependents = 3;

    // --- Scenario 1: Main thread waits for multiple worker threads ---
    std::cout << "\nScenario 1: Main thread waits for " << num_workers << " workers." << std::endl;
    std::latch workers_finished_latch(num_workers); // Initialize latch with worker count

    std::vector<std::thread> worker_threads;
    for (int i = 0; i < num_workers; ++i) {
        int duration = 50 + (i * 20); // Varying work durations
        worker_threads.emplace_back(worker_task, i + 1, std::ref(workers_finished_latch), duration);
    }

    std::osyncstream(std::cout) << "Main thread: All workers launched. Waiting for them to finish..." << std::endl;
    workers_finished_latch.wait(); // Main thread blocks until latch counter is zero

    std::osyncstream(std::cout) << "Main thread: All workers have finished. Proceeding." << std::endl;

    for (std::thread& t : worker_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    worker_threads.clear(); // Clear for next scenario


    // --- Scenario 2: Multiple dependent tasks wait for a setup phase ---
    std::cout << "\nScenario 2: " << num_dependents << " dependent tasks wait for a 'setup' latch (count 1)." << std::endl;
    std::latch setup_complete_latch(1); // Latch with count 1, representing a single event

    std::vector<std::thread> dependent_threads;
    for (int i = 0; i < num_dependents; ++i) {
        dependent_threads.emplace_back(dependent_task, i + 1, std::ref(setup_complete_latch));
    }

    std::osyncstream(std::cout) << "Main thread: Dependent tasks launched. Simulating setup phase (200ms)..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    std::osyncstream(std::cout) << "Main thread: Setup phase complete. Signaling latch (count_down)." << std::endl;
    setup_complete_latch.count_down(); // Decrements counter to 0, unblocking dependent tasks

    for (std::thread& t : dependent_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    dependent_threads.clear();

    // --- Scenario 3: Using arrive_and_wait ---
    // All threads arrive at the latch, decrement, and then all wait until counter is zero.
    // Useful for synchronizing a group of threads at a specific point.
    std::cout << "\nScenario 3: " << num_workers << " threads using arrive_and_wait." << std::endl;
    std::latch rendezvous_latch(num_workers);

    for (int i = 0; i < num_workers; ++i) {
        worker_threads.emplace_back([i, &rendezvous_latch]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(20 * i)); // Stagger arrival slightly
            std::osyncstream(std::cout) << "Thread " << i + 1 << " arriving at rendezvous latch." << std::endl;
            rendezvous_latch.arrive_and_wait(); // Decrement and wait
            std::osyncstream(std::cout) << "Thread " << i + 1 << " passed rendezvous latch." << std::endl;
        });
    }

    for (std::thread& t : worker_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    worker_threads.clear();

    std::cout << "\nstd::latch example finished." << std::endl;
    return 0;
}

/*
Explanation:
`std::latch` (from the `<latch>` header) is a C++20 synchronization primitive
that provides a way for one or more threads to wait until a counter is decremented
to zero. It's a single-use barrier: once the counter reaches zero, it cannot be reset.

Key Features:
1.  Counter-Based:
    -   Initialized with an expected count.
    -   Threads can decrement this counter.
    -   Threads can wait until the counter reaches zero.

2.  Single-Use:
    -   Once the counter reaches zero, it remains zero. The latch cannot be reset
        or reused for another synchronization phase. For reusable barriers,
        `std::barrier` (also C++20) should be used.

3.  Main Operations:
    -   `std::latch my_latch(count);`: Constructor, initializes the internal counter to `count`.
        `count` must be non-negative.
    -   `my_latch.count_down(n = 1);`: Atomically decrements the internal counter by `n`.
        If the counter reaches zero as a result, all threads waiting on the latch are unblocked.
    -   `my_latch.wait();`: Blocks the calling thread until the internal counter reaches zero.
        If the counter is already zero when `wait()` is called, it returns immediately.
    -   `my_latch.arrive_and_wait(n = 1);`: Atomically decrements the counter by `n`
        (like `count_down(n)`) and then blocks until the counter reaches zero (like `wait()`).
        This is useful when a thread performs its part of the work and then needs to
        wait for all other participating threads to also complete their parts.
    -   `my_latch.try_wait() noexcept;` (C++20): Checks if the internal counter has reached zero.
        Returns `true` if the counter is zero, `false` otherwise. Does not block. (Not shown in example)

Use Cases:
-   Coordinating the start of multiple threads: Initialize a latch with 1. Worker
    threads `wait()` on it. The main thread does setup and then calls `count_down()`
    to release all workers simultaneously. (Scenario 2 in example)
-   Waiting for completion of multiple tasks: Initialize a latch with the number
    of tasks. Each task calls `count_down()` when it finishes. The main thread
    `wait()`s for all tasks to complete. (Scenario 1 in example)
-   Rendezvous point: All participating threads call `arrive_and_wait()`. They all
    block until the last thread arrives, after which all are unblocked. (Scenario 3 in example)

`std::latch` vs. `std::barrier`:
-   `std::latch`: Single-use. Counter goes down to zero.
-   `std::barrier`: Reusable. Threads arrive, and when the expected number of
    threads arrive, a completion phase function can be executed, and then the
    barrier resets for the next phase. More complex, for cyclic synchronization.

Thread Safety with Output:
-   The example uses `std::osyncstream(std::cout)` for printing from multiple
    threads. `std::osyncstream` (C++20, from `<syncstream>`) ensures that output
    operations are atomic with respect to each other, preventing interleaved output
    from different threads on standard streams like `std::cout`.

How to compile:
g++ -std=c++20 std_latch.cpp -o std_latch_example -pthread
(or clang++ -std=c++20 ...)
The `-pthread` flag is needed for `std::thread`.
Ensure your compiler and standard library fully support C++20 latches and syncstream.
*/
