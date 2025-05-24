// std_chrono.cpp
#include <iostream>
#include <chrono>   // Main header for chrono library
#include <thread>   // For std::this_thread::sleep_for
#include <iomanip>  // For std::put_time (C++11, but formatting might be more robust in later standards)
#include <ctime>    // For std::time_t, std::localtime, std::gmtime

// Helper function to print duration in various units
template<typename Duration>
void print_duration_details(const Duration& d) {
    std::cout << "  In nanoseconds:  " << std::chrono::duration_cast<std::chrono::nanoseconds>(d).count() << " ns\n";
    std::cout << "  In microseconds: " << std::chrono::duration_cast<std::chrono::microseconds>(d).count() << " us\n";
    std::cout << "  In milliseconds: " << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() << " ms\n";
    std::cout << "  In seconds:      " << std::chrono::duration_cast<std::chrono::seconds>(d).count() << " s\n";
    // For floating point seconds:
    std::chrono::duration<double> d_sec = d;
    std::cout << "  In floating-point seconds: " << d_sec.count() << " s\n";
}

int main() {
    // --- 1. Durations ---
    // std::chrono::duration represents a time interval.
    // It's a template: std::chrono::duration<Rep, Period>
    // - Rep: The type used to store the count of ticks (e.g., int, long, double).
    // - Period: A std::ratio representing the tick period in seconds (e.g., std::milli for milliseconds).

    std::cout << "--- Durations ---" << std::endl;
    std::chrono::seconds s(5);                     // 5 seconds
    std::chrono::milliseconds ms = std::chrono::milliseconds(1500); // 1500 milliseconds
    std::chrono::microseconds us = s + ms;         // Durations can be added/subtracted if types are compatible
                                                   // Result type is determined by common type logic.
    std::chrono::nanoseconds ns(us);               // Implicit conversion if no precision loss

    std::cout << "Duration s: " << s.count() << " seconds" << std::endl;
    print_duration_details(s);
    std::cout << "Duration ms: " << ms.count() << " milliseconds" << std::endl;
    print_duration_details(ms);
    std::cout << "Duration us (s + ms): " << us.count() << " microseconds" << std::endl;
    print_duration_details(us);

    // Predefined duration types:
    // std::chrono::hours, minutes, seconds, milliseconds, microseconds, nanoseconds

    // Example: using a floating point representation for duration
    std::chrono::duration<double, std::ratio<1, 1>> fp_seconds(2.75); // 2.75 seconds
    std::cout << "Floating point seconds: " << fp_seconds.count() << "s" << std::endl;
    print_duration_details(fp_seconds);


    // --- 2. Clocks ---
    // Clocks provide information about the current time (time points).
    // C++11 provides three main clocks:
    // a. std::chrono::system_clock: Wall clock time from the system-wide realtime clock.
    //    Can be adjusted (e.g., by NTP), so not always monotonic.
    //    Provides to_time_t() and from_time_t() for conversion to C-style time.
    // b. std::chrono::steady_clock: Monotonic clock. Cannot be adjusted.
    //    Best for measuring time intervals (e.g., for performance timing).
    //    Epoch is often system boot time.
    // c. std::chrono::high_resolution_clock: Clock with the smallest possible tick period.
    //    It's often an alias for system_clock or steady_clock. (May not be steady!)

    std::cout << "\n--- Clocks & Time Points ---" << std::endl;

    // system_clock
    std::chrono::time_point<std::chrono::system_clock> sys_now = std::chrono::system_clock::now();
    // Convert to C-style time_t for printing
    std::time_t sys_now_c = std::chrono::system_clock::to_time_t(sys_now);
    // std::put_time is C++11 but locale dependent and might be tricky with some compilers/platforms
    // For more robust formatting, C++20 <format> is better.
    std::cout << "system_clock::now(): " << std::put_time(std::localtime(&sys_now_c), "%Y-%m-%d %H:%M:%S %Z") << std::endl;
    
    // Get duration since epoch for system_clock
    std::cout << "system_clock duration since epoch: " << sys_now.time_since_epoch().count() 
              << " ticks (period: " << std::chrono::system_clock::period::num << "/" 
              << std::chrono::system_clock::period::den << "s)" << std::endl;


    // steady_clock (for measuring intervals)
    std::chrono::time_point<std::chrono::steady_clock> steady_start = std::chrono::steady_clock::now();
    std::cout << "Simulating work for 100ms using steady_clock..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep using chrono duration
    std::chrono::time_point<std::chrono::steady_clock> steady_end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed_seconds = steady_end - steady_start;
    std::chrono::milliseconds elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(steady_end - steady_start);

    std::cout << "Work took: " << elapsed_seconds.count() << " s" << std::endl;
    std::cout << "Work took: " << elapsed_milliseconds.count() << " ms" << std::endl;
    
    // Check if steady_clock is indeed steady (monotonic)
    std::cout << "steady_clock is_steady: " << std::boolalpha << std::chrono::steady_clock::is_steady << std::endl;


    // high_resolution_clock
    std::cout << "high_resolution_clock is_steady: " << std::boolalpha << std::chrono::high_resolution_clock::is_steady << std::endl;
    std::chrono::time_point<std::chrono::high_resolution_clock> hr_now = std::chrono::high_resolution_clock::now();
    std::cout << "high_resolution_clock duration since epoch: " << hr_now.time_since_epoch().count() 
              << " ticks (period: " << std::chrono::high_resolution_clock::period::num << "/" 
              << std::chrono::high_resolution_clock::period::den << "s)" << std::endl;


    // --- 3. Time Points ---
    // std::chrono::time_point represents a point in time.
    // It's a template: std::chrono::time_point<Clock, Duration = typename Clock::duration>
    // It's essentially a duration relative to a clock's epoch.

    std::cout << "\n--- Time Point Arithmetic ---" << std::endl;
    std::chrono::time_point<std::chrono::system_clock> tp1 = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> tp2 = tp1 + std::chrono::seconds(60); // 1 minute later

    std::time_t tp1_c = std::chrono::system_clock::to_time_t(tp1);
    std::time_t tp2_c = std::chrono::system_clock::to_time_t(tp2);

    std::cout << "Time point 1: " << std::put_time(std::localtime(&tp1_c), "%H:%M:%S") << std::endl;
    std::cout << "Time point 2 (tp1 + 60s): " << std::put_time(std::localtime(&tp2_c), "%H:%M:%S") << std::endl;

    if (tp2 > tp1) {
        std::cout << "tp2 is later than tp1." << std::endl;
    }
    std::chrono::duration<double> diff = tp2 - tp1;
    std::cout << "Difference between tp2 and tp1: " << diff.count() << " s" << std::endl;


    // --- 4. Literals (C++14, but useful to know about for chrono) ---
    // C++14 introduced user-defined literals for chrono durations:
    // using namespace std::chrono_literals; // (Needs C++14)
    // auto d1 = 10s;  // 10 seconds
    // auto d2 = 200ms; // 200 milliseconds
    // auto d3 = 50us; // 50 microseconds
    // This example is C++11, so literals are not used here.
    std::cout << "\n(C++14 chrono literals like '10s' or '200ms' are not used in this C++11 example.)" << std::endl;

    std::cout << "\n--- Example: Timing a function ---" << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Simulate some work
    long long temp_sum = 0;
    for(int i = 0; i < 1000000; ++i) {
        temp_sum += i;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time_taken = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    std::cout << "Simulated work (summing to 1M-1) took: " << time_taken.count() << " microseconds." << std::endl;
    std::cout << "(Sum result: " << temp_sum << ", to ensure work is not optimized away)" << std::endl;

    return 0;
}

/*
Explanation:
The `<chrono>` library (introduced in C++11) provides a flexible and type-safe
way to work with time durations, time points, and clocks.

Core Components:

1.  `std::chrono::duration`:
    -   Represents a time interval (e.g., 5 seconds, 100 milliseconds).
    -   Template: `duration<Rep, Period>`, where `Rep` is the numerical type
        for the count of ticks (e.g., `long`, `double`), and `Period` is a
        `std::ratio` representing the tick period in seconds (e.g., `std::milli`
        for 1/1000 seconds).
    -   Predefined typedefs: `hours`, `minutes`, `seconds`, `milliseconds`,
        `microseconds`, `nanoseconds`.
    -   Supports arithmetic operations (+, -, *, / with scalars).
    -   `count()` member function returns the number of ticks.
    -   `std::chrono::duration_cast<ToDuration>(d)` converts a duration `d` to
        type `ToDuration` (truncating if necessary).

2.  `std::chrono::time_point`:
    -   Represents a specific point in time (e.g., "now", or a specific calendar date/time).
    -   Template: `time_point<Clock, Duration = typename Clock::duration>`.
        It's essentially a duration measured from a particular clock's epoch.
    -   `time_since_epoch()` member function returns the duration from the clock's
        epoch to this time point.
    -   Supports arithmetic with durations (e.g., `time_point + duration = time_point`).
    -   Can be compared (`<`, `>`, `==`, etc.).

3.  Clocks:
    Clocks are sources of time points. C++11 provides three main clocks:
    a.  `std::chrono::system_clock`:
        -   Represents the system's real-time wall clock.
        -   It's the only clock that can be easily converted to/from C-style
            `std::time_t` (using `to_time_t()` and `from_time_t()`). This makes
            it suitable for calendar time display.
        -   Not necessarily monotonic (i.e., it can go backward if the system
            time is adjusted).
        -   Epoch is usually the UNIX epoch (Jan 1, 1970 UTC).
    b.  `std::chrono::steady_clock`:
        -   A monotonic clock, meaning its value never decreases as physical time
            advances.
        -   The best choice for measuring time intervals (e.g., performance timing,
            timeouts) because it's not affected by system time adjustments.
        -   Epoch is usually system boot time or some other fixed point.
    c.  `std::chrono::high_resolution_clock`:
        -   The clock with the shortest possible tick period available on the system.
        -   It's often an alias for `system_clock` or `steady_clock`.
        -   Check `is_steady` member to know if it's monotonic.

Usage:
-   Measuring time: Record `steady_clock::now()` before and after an operation,
    then subtract the time points to get a duration.
-   Sleeping/Waiting: `std::this_thread::sleep_for(duration)` and
    `std::this_thread::sleep_until(time_point)`.
-   Interfacing with C API: Convert `system_clock::time_point` to `std::time_t`
    for use with functions like `std::localtime` or `std::strftime`.

C++14 and later add convenient chrono literals (e.g., `10s`, `200ms`), which
simplify writing duration values.

How to compile:
g++ -std=c++11 std_chrono.cpp -o std_chrono_example
./std_chrono_example
(No special linking flags like -pthread are typically needed for chrono itself,
unless used in conjunction with std::thread that requires it).
*/
