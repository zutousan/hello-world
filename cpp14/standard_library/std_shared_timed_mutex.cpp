// std_shared_timed_mutex.cpp
#include <iostream>
#include <thread>
#include <mutex>          // For std::mutex, std::lock_guard, std::unique_lock
#include <shared_mutex>   // For std::shared_timed_mutex (C++14), std::shared_lock (C++14)
#include <vector>
#include <chrono>
#include <sstream>      // For std::ostringstream
#include <string>       // For std::to_string, std::string
#include <algorithm>    // For std::count
#include <ctime>        // For time (for srand)

// A shared resource
struct SharedData {
    int value = 0;
    std::string log;
    mutable std::shared_timed_mutex mutex_;

    void add_log(const std::string& entry) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        log += entry;
        log += "\n"; // Corrected newline
    }
};

// Function for threads that write to SharedData
void writer_thread(SharedData& data, int id, int num_writes) {
    std::ostringstream tid_stream;
    tid_stream << std::this_thread::get_id();
    std::string tid_str = tid_stream.str();

    for (int i = 0; i < num_writes; ++i) {
        std::unique_lock<std::shared_timed_mutex> lock(data.mutex_);
            data.value++;
            std::string entry = "Writer " + std::to_string(id) + " (TID: " + tid_str.substr(0,4) + 
                                ") wrote value: " + std::to_string(data.value);
            data.add_log(entry);
            std::cout << entry << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10 + (rand() % 20)));
    }
}

// Function for threads that read from SharedData
void reader_thread(const SharedData& data, int id, int num_reads) {
    std::ostringstream tid_stream;
    tid_stream << std::this_thread::get_id();
    std::string tid_str = tid_stream.str();

    for (int i = 0; i < num_reads; ++i) {
        std::shared_lock<std::shared_timed_mutex> lock(data.mutex_);
            std::string entry = "Reader " + std::to_string(id) + " (TID: " + tid_str.substr(0,4) + 
                                ") read value: " + std::to_string(data.value);
            std::cout << entry << " (Log entries: " << std::count(data.log.begin(), data.log.end(), '\n') << ")" << std::endl; // Corrected count for newline
        std::this_thread::sleep_for(std::chrono::milliseconds(5 + (rand() % 10)));
    }
}

int main() {
    SharedData shared_resource;
    srand(static_cast<unsigned int>(time(nullptr)));

    std::cout << "--- std::shared_timed_mutex Demo ---" << std::endl;
    std::cout << "Launching 2 writer threads and 5 reader threads." << std::endl;

    std::vector<std::thread> threads;

    threads.emplace_back(writer_thread, std::ref(shared_resource), 1, 3);
    threads.emplace_back(writer_thread, std::ref(shared_resource), 2, 2);

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(reader_thread, std::cref(shared_resource), i + 1, 5);
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "\n--- Final Log Content ---" << std::endl; // Corrected newline
    {
        std::shared_lock<std::shared_timed_mutex> lock(shared_resource.mutex_);
        std::cout << shared_resource.log << std::endl;
        std::cout << "Final value: " << shared_resource.value << std::endl;
    }

    std::cout << "\n--- Timed Lock Attempts (Conceptual) ---" << std::endl; // Corrected newline
    std::shared_timed_mutex timed_mtx;
    
    std::unique_lock<std::shared_timed_mutex> ex_lock(timed_mtx, std::try_to_lock);
    if (ex_lock.owns_lock()) {
        std::cout << "Exclusive lock acquired immediately via try_to_lock." << std::endl;
        ex_lock.unlock();
    } else {
        std::cout << "Could not acquire exclusive lock immediately via try_to_lock." << std::endl;
    }

    std::thread t_holder([&]() {
        std::unique_lock<std::shared_timed_mutex> lk(timed_mtx);
        std::cout << "(Thread t_holder now holds the exclusive lock for 200ms)" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50)); 

    std::cout << "Main attempting try_lock_for (exclusive) for 100ms..." << std::endl;
    if (timed_mtx.try_lock_for(std::chrono::milliseconds(100))) {
        std::cout << "Main acquired exclusive lock via try_lock_for." << std::endl;
        timed_mtx.unlock();
    } else {
        std::cout << "Main failed to acquire exclusive lock via try_lock_for within 100ms." << std::endl;
    }
    
    if (t_holder.joinable()) t_holder.join();


    std::thread t_shared_holder([&]() {
        std::shared_lock<std::shared_timed_mutex> slk(timed_mtx);
        std::cout << "(Thread t_shared_holder now holds a shared lock for 200ms)" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    std::cout << "Main attempting try_lock_shared_for for 100ms..." << std::endl;
    if (timed_mtx.try_lock_shared_for(std::chrono::milliseconds(100))) {
        std::cout << "Main acquired shared lock via try_lock_shared_for." << std::endl;
        timed_mtx.unlock_shared();
    } else {
        std::cout << "Main failed to acquire shared lock via try_lock_shared_for within 100ms." << std::endl;
    }

    if (t_shared_holder.joinable()) t_shared_holder.join();

    return 0;
}

/*
Explanation:
`std::shared_timed_mutex` (introduced in C++14, in `<shared_mutex>`) is a
synchronization primitive that allows multiple threads to read a shared resource
concurrently, or one thread to write to it exclusively. It extends the
functionality of a regular mutex (`std::mutex`) and a shared mutex
(`std::shared_mutex`, C++17) by adding timed locking capabilities.

It supports three levels of access:
1.  Exclusive Access (Write Lock):
    -   Only one thread can hold an exclusive lock at any time.
    -   While an exclusive lock is held, no other thread (neither reader nor writer)
        can acquire any lock (shared or exclusive).
    -   Typically acquired using `std::unique_lock` or `std::lock_guard`.
    -   Methods:
        - `lock()`: Blocks until exclusive lock is acquired.
        - `try_lock()`: Attempts to acquire lock, returns immediately (bool).
        - `try_lock_for(duration)`: Attempts to acquire lock, blocks for up to `duration`. (C++14)
        - `try_lock_until(time_point)`: Attempts to acquire lock, blocks until `time_point`. (C++14)
        - `unlock()`: Releases the exclusive lock.

2.  Shared Access (Read Lock):
    -   Multiple threads can hold a shared lock concurrently.
    -   Shared locks can be acquired as long as no thread holds an exclusive lock.
    -   Typically acquired using `std::shared_lock<std::shared_timed_mutex>` (C++14).
    -   Methods:
        - `lock_shared()`: Blocks until shared lock is acquired.
        - `try_lock_shared()`: Attempts to acquire shared lock, returns immediately.
        - `try_lock_shared_for(duration)`: Attempts, blocks for up to `duration`. (C++14)
        - `try_lock_shared_until(time_point)`: Attempts, blocks until `time_point`. (C++14)
        - `unlock_shared()`: Releases a shared lock.

Use Cases (Readers-Writer Lock):
-   Protecting a data structure that is read frequently by multiple threads but
    written to infrequently by a few threads.
-   Readers can access the data concurrently, improving performance.
-   Writers get exclusive access, ensuring data integrity during modifications.

RAII Wrappers:
-   `std::unique_lock<std::shared_timed_mutex>`: For managing exclusive locks (provides
    more flexibility than `std::lock_guard`, e.g., timed attempts, deferred locking).
-   `std::shared_lock<std::shared_timed_mutex>` (C++14): For managing shared locks
    (similar to `std::unique_lock` but for shared access).

Timed Operations:
The "-timed-" part means its `try_lock_...` methods can wait for a specified
duration or until a specific time point, which is useful for preventing indefinite
blocking and for implementing timeouts.

Comparison with `std::shared_mutex` (C++17):
-   `std::shared_mutex` (C++17) is similar but does *not* provide the `_for` and
    `_until` timed locking methods.
-   `std::shared_timed_mutex` is the one available in C++14 for shared locking with timed capabilities.

How to compile:
g++ -std=c++14 std_shared_timed_mutex.cpp -o shared_timed_mutex_example -pthread
./shared_timed_mutex_example
*/
