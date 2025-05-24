// std_thread.cpp
#include <iostream>
#include <thread>   // For std::thread
#include <vector>
#include <chrono>   // For std::chrono::milliseconds
#include <numeric>  // For std::iota (in C++20, also in <numbers>)

// A simple function to be executed by a thread
void task_function(int id, int duration_ms) {
    std::cout << "Thread " << id << ": Starting task." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
    std::cout << "Thread " << id << ": Finished task after " << duration_ms << "ms." << std::endl;
}

// A class with a member function to be executed by a thread
class MyWorker {
public:
    int id;
    MyWorker(int i) : id(i) {}

    void process(int data) {
        std::cout << "Worker " << id << ": Processing data " << data << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * data));
        std::cout << "Worker " << id << ": Finished processing data " << data << std::endl;
    }
};

// Function to demonstrate returning a value (via reference or promise/future)
void sum_vector(const std::vector<int>& v, long long& result) { // Using reference to return sum
    long long current_sum = 0;
    for (int val : v) {
        current_sum += val;
    }
    result = current_sum;
    std::cout << "Thread (sum_vector): Calculated sum = " << result << std::endl;
}


int main() {
    std::cout << "--- Basic Thread Creation and Joining ---" << std::endl;

    // 1. Creating a thread with a free function
    std::thread t1(task_function, 1, 200); // Pass function and its arguments

    // 2. Creating a thread with a lambda expression
    std::thread t2([](int id) {
        std::cout << "Thread " << id << " (lambda): Starting." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::cout << "Thread " << id << " (lambda): Exiting." << std::endl;
    }, 2);

    // 3. Creating a thread with a class member function
    MyWorker worker_obj(3);
    // For member functions, pass a pointer to the member function,
    // a pointer/reference to the object, and then the arguments.
    std::thread t3(&MyWorker::process, &worker_obj, 5); // Process data 5
    // If worker_obj was created with new: std::thread t3(&MyWorker::process, worker_ptr, 5);

    std::cout << "Main thread: All threads launched." << std::endl;

    // Wait for threads to complete their execution
    // If join() is not called, the std::thread destructor will call std::terminate
    // if the thread is still joinable.
    t1.join();
    std::cout << "Main thread: t1 joined." << std::endl;
    t2.join();
    std::cout << "Main thread: t2 joined." << std::endl;
    t3.join();
    std::cout << "Main thread: t3 joined." << std::endl;

    std::cout << "\n--- Detaching a Thread (Use with Caution) ---" << std::endl;
    // std::thread t_detach(task_function, 4, 500);
    // t_detach.detach(); // Detached threads run independently.
                       // Main program must ensure resources they access are valid.
                       // If main exits, detached threads are terminated.
    // std::cout << "Main thread: t_detach launched and detached." << std::endl;
    // If you uncomment the detach example, add a sleep in main to see it run,
    // e.g., std::this_thread::sleep_for(std::chrono::seconds(1));
    // Otherwise, main might exit before the detached thread does much.
    std::cout << "(Detach example commented out for predictable output without sleeps in main)" << std::endl;


    std::cout << "\n--- Getting Thread ID ---" << std::endl;
    std::thread t_id_example(task_function, 5, 50);
    std::cout << "Main thread ID: " << std::this_thread::get_id() << std::endl;
    std::cout << "t_id_example thread ID: " << t_id_example.get_id() << std::endl;
    t_id_example.join();


    std::cout << "\n--- Returning a value from a thread (via reference) ---" << std::endl;
    std::vector<int> data_to_sum(1000);
    std::iota(data_to_sum.begin(), data_to_sum.end(), 1); // Fill with 1, 2, ..., 1000
    long long sum_result = 0;
    std::thread t_sum(sum_vector, std::cref(data_to_sum), std::ref(sum_result));
    // std::cref is used for const references, std::ref for non-const.
    // This is important because thread constructor copies its arguments by default.
    // std::ref ensures that the actual sum_result variable is passed by reference.
    t_sum.join();
    std::cout << "Main thread: Sum calculated by t_sum = " << sum_result << std::endl;
    // Note: For more complex return values and synchronization, std::promise and std::future are preferred.


    std::cout << "\n--- Hardware Concurrency ---" << std::endl;
    unsigned int n_cores = std::thread::hardware_concurrency();
    std::cout << "Number of concurrent threads supported (hint): " << n_cores << std::endl;
    // This is a hint and might return 0 if not determinable.

    std::cout << "\nMain thread: Program finished." << std::endl;
    return 0;
}

/*
Explanation:
`std::thread` (in `<thread>` header) is the C++11 standard library facility for
creating and managing threads of execution.

Key Concepts:
1.  Creating Threads:
    -   An `std::thread` object is created by passing a callable entity (function
        pointer, lambda, function object) and its arguments to the `std::thread`
        constructor.
    -   The thread starts executing the callable entity immediately upon creation.
    -   Arguments are copied/moved to internal storage for the new thread. Use
        `std::ref` or `std::cref` (from `<functional>`) if you need to pass
        arguments by reference.

2.  Joining Threads (`join()`):
    -   `t.join()`: The calling thread (e.g., the main thread) blocks until the
        thread `t` completes its execution.
    -   It's crucial to `join()` a thread or `detach()` it before the `std::thread`
        object is destroyed. If a `std::thread` object is destroyed while it's
        still "joinable" (i.e., neither `join()` nor `detach()` has been called),
        `std::terminate()` is called, aborting the program.
    -   A thread can only be joined once. After `join()` returns, `t.joinable()`
        will be `false`.

3.  Detaching Threads (`detach()`):
    -   `t.detach()`: Separates the thread of execution from the `std::thread`
        object. The thread continues to run independently in the background.
    -   Once detached, the thread cannot be joined, and the `std::thread` object
        is no longer associated with that thread of execution (`t.joinable()`
        becomes `false`).
    -   Detached threads must be managed carefully:
        -   Ensure they don't outlive resources they access (e.g., objects from
            the main thread's scope).
        -   If `main()` exits, all detached threads are abruptly terminated.
    -   Generally, `join()` is preferred for better control unless you have a
        specific reason for "fire and forget" daemon-like threads.

4.  Thread Local Storage:
    -   The `thread_local` keyword can be used to declare variables that have a
        separate instance for each thread (not shown in this basic example).

5.  `std::this_thread` Namespace:
    -   `std::this_thread::get_id()`: Gets the ID of the current thread.
    -   `std::this_thread::sleep_for()`: Pauses the current thread for a duration.
    -   `std::this_thread::sleep_until()`: Pauses the current thread until a time point.
    -   `std::this_thread::yield()`: Hints to the scheduler to run other threads.

6.  Passing Arguments:
    -   Arguments are copied by default. If you pass a pointer, the pointer is
        copied, not the data it points to (careful with lifetimes).
    -   Use `std::ref(variable)` or `std::cref(variable)` to pass arguments
        by reference or const reference, respectively.

7.  Returning Values:
    -   `std::thread` itself doesn't directly provide a mechanism for a thread to
        return a value to the caller. Common ways to achieve this:
        -   Pass a reference to a variable where the thread can store the result
            (requires careful synchronization if multiple threads write or if
            the main thread reads while the worker writes).
        -   Use `std::promise` and `std::future` (more advanced, part of C++11
            concurrency support, provides a cleaner way to get results and handle
            exceptions from threads). This is generally the preferred method.

8.  `std::thread::hardware_concurrency()`:
    -   Returns an estimate of the number of hardware thread contexts (e.g.,
        number of cores or hyper-threads). It's a hint and can be 0 if unknown.

Compilation:
-   When compiling programs using `std::thread`, you typically need to link
    against the pthreads library (on POSIX systems like Linux).
    `g++ -std=c++11 std_thread.cpp -o std_thread_example -pthread`
    (On some systems/compilers, `-pthread` might not be strictly necessary if
    the compiler handles it by default with `-std=c++11` or newer, but it's
    good practice to include it.)

Considerations:
-   Synchronization: When multiple threads access shared data, you must use
    synchronization primitives (mutexes, condition variables, atomics) to prevent
    race conditions. These are covered in other parts of the C++ standard library.
-   Exception Handling: Exceptions thrown in a thread that are not caught within
    that thread will typically call `std::terminate`. `std::promise`/`std::future`
    can help propagate exceptions.

How to compile:
g++ -std=c++11 std_thread.cpp -o std_thread_example -pthread
./std_thread_example
*/
