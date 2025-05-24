// std_span.cpp
#include <iostream>
#include <vector>
#include <array>
#include <span>     // Key header for std::span (C++20)
#include <string>   // For using with std::string_view potentially, or C-style strings

// Function that processes a contiguous sequence of integers
// Using std::span allows this function to accept various data sources
// without templates or overloading for each container type.
void print_sum_and_elements(std::span<const int> data_view, const std::string& title) {
    std::cout << title << ":" << std::endl;
    long long sum = 0;
    std::cout << "  Elements: [";
    bool first = true;
    for (int x : data_view) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << x;
        sum += x;
        first = false;
    }
    std::cout << "]" << std::endl;
    std::cout << "  Sum: " << sum << std::endl;
    std::cout << "  Number of elements (span.size()): " << data_view.size() << std::endl;
    std::cout << "  Size in bytes (span.size_bytes()): " << data_view.size_bytes() << std::endl;
}

// Function that modifies elements through a span (if not const span)
void double_elements(std::span<int> data_view) {
    for (int& x : data_view) {
        x *= 2;
    }
}

int main() {
    std::cout << "--- std::span (C++20) ---" << std::endl;

    // 1. Creating spans from various sources
    std::cout << "\n1. Creating spans:" << std::endl;

    // From a C-style array
    int c_array[] = {1, 2, 3, 4, 5};
    std::span<int> span_from_c_array(c_array); // Deduces size
    // std::span<const int> span_from_c_array_const(c_array); // Can also be const
    print_sum_and_elements(span_from_c_array, "Span from C-array");

    // From a std::vector
    std::vector<int> vec = {10, 20, 30, 40, 50, 60};
    std::span<int> span_from_vector(vec);
    print_sum_and_elements(span_from_vector, "Span from std::vector");

    // From a std::array
    std::array<int, 4> std_arr = {100, 200, 300, 400};
    std::span<const int> span_from_std_array(std_arr.begin(), std_arr.end()); // Or just std_arr
    print_sum_and_elements(span_from_std_array, "Span from std::array");

    // From a pointer and size
    int* raw_ptr = vec.data() + 1; // Point to the second element of vec
    size_t raw_size = 3;
    std::span<int> span_from_ptr_size(raw_ptr, raw_size); // {20, 30, 40}
    print_sum_and_elements(span_from_ptr_size, "Span from raw pointer and size");

    // Empty span
    std::span<int> empty_span;
    std::cout << "Empty span size: " << empty_span.size() << ", empty? " << std::boolalpha << empty_span.empty() << std::endl;


    // 2. Subspans (creating views into parts of a span)
    std::cout << "\n2. Subspans:" << std::endl;
    std::span<int> main_span = vec; // {10, 20, 30, 40, 50, 60}

    // First N elements: .first(N)
    std::span<int> first_3 = main_span.first(3); // {10, 20, 30}
    print_sum_and_elements(first_3, "Subspan: first 3 elements of vector span");

    // Last N elements: .last(N)
    std::span<int> last_2 = main_span.last(2); // {50, 60}
    print_sum_and_elements(last_2, "Subspan: last 2 elements of vector span");

    // Subspan from an offset with a count: .subspan(offset, count)
    // count can be std::dynamic_extent to go until the end.
    std::span<int> middle_part = main_span.subspan(2, 3); // Elements at index 2, 3, 4 -> {30, 40, 50}
    print_sum_and_elements(middle_part, "Subspan: middle 3 elements (offset 2, count 3)");

    std::span<int> from_offset_to_end = main_span.subspan(3); // Elements from index 3 to end -> {40, 50, 60}
    print_sum_and_elements(from_offset_to_end, "Subspan: from offset 3 to end");


    // 3. Modifying data through a non-const span
    std::cout << "\n3. Modifying data via span:" << std::endl;
    std::vector<int> mod_vec = {1, 2, 3, 4};
    print_sum_and_elements(mod_vec, "Original mod_vec");
    
    std::span<int> mod_span(mod_vec);
    double_elements(mod_span); // Pass span to function that modifies elements
    print_sum_and_elements(mod_vec, "mod_vec after double_elements via span");

    // Modifying a subspan also modifies the original data
    std::span<int> mod_sub_span = mod_span.subspan(1, 2); // Elements at index 1, 2 of mod_vec (now {4, 6})
    mod_sub_span[0] = 99; // Modifies mod_vec[1]
    print_sum_and_elements(mod_vec, "mod_vec after mod_sub_span[0] = 99");


    // 4. Converting span to bytes
    std::cout << "\n4. Span as bytes:" << std::endl;
    std::span<const std::byte> byte_view = std::as_bytes(span_from_c_array);
    std::cout << "Byte view of C-array span (first 4 bytes): [";
    for(size_t i = 0; i < byte_view.size() && i < 4; ++i) {
        std::cout << std::hex << std::showbase << static_cast<int>(byte_view[i]) << (i==3 ? "" : ", ");
    }
    std::cout << "...]" << std::dec << std::noshowbase << std::endl;

    // std::as_writable_bytes (for non-const data)
    std::span<std::byte> writable_byte_view = std::as_writable_bytes(mod_span);
    if (!writable_byte_view.empty()) {
        writable_byte_view[0] = std::byte{0xFF}; // Modify the first byte of the first int in mod_span
    }
    print_sum_and_elements(mod_vec, "mod_vec after modifying first byte via writable_byte_view");


    // 5. Fixed-size spans (std::span<T, Extent>)
    std::cout << "\n5. Fixed-size spans:" << std::endl;
    // std::span<int, 5> fixed_span_c_array(c_array); // OK, c_array has 5 elements
    // The above line can be written as:
    std::span<int, std::dynamic_extent> dyn_span = c_array; // This is what we've mostly used
    std::span<int, 5> static_span(c_array, 5); // Explicitly state known extent

    std::cout << "Static span size: " << static_span.size() << " (compile-time constant)" << std::endl;
    // static_span = vec; // Compile error: cannot assign dynamic extent span to static extent span if sizes mismatch
                        // or if source is not guaranteed to have that size at compile time.
    // std::span<int, 3> wrong_size_span(c_array, 5); // Compile error if Extent != size argument and Extent is fixed

    // Using fixed-size span with std::array (size is known at compile time)
    std::array<int, 4> arr_for_fixed_span = {1,2,3,4};
    std::span<int, 4> fs_from_std_array = arr_for_fixed_span;
    print_sum_and_elements(fs_from_std_array, "Fixed-size span from std::array");


    std::cout << "\nstd::span provides a safe, non-owning view into contiguous data." << std::endl;
    return 0;
}

/*
Explanation:
`std::span<T, Extent>` (from the `<span>` header) is a C++20 feature that provides
a non-owning "view" or "reference" to a contiguous sequence of objects of type `T`.
`Extent` is an optional compile-time constant specifying the number of elements;
if omitted or `std::dynamic_extent` is used, the size is determined at runtime.

Key Characteristics:
1.  Non-Owning:
    -   A `std::span` does not own the data it points to. It's like a raw pointer
        and a size, but with bounds safety and a richer interface.
    -   The lifetime of the underlying data must be managed separately and must
        outlive the `span`. Using a `span` with a dangling pointer leads to
        undefined behavior.

2.  View into Contiguous Data:
    -   It can refer to C-style arrays, `std::vector`, `std::array`, a sub-section
        of these, or any other contiguous sequence of objects.

3.  Compile-Time vs. Runtime Size:
    -   `std::span<T>` (or `std::span<T, std::dynamic_extent>`): Size is determined
        and stored at runtime.
    -   `std::span<T, N>` (where N is an integer): Size is fixed at compile time.
        This can lead to optimizations and compile-time checks.

4.  Interface:
    -   `size()`: Number of elements.
    -   `size_bytes()`: Total size in bytes.
    -   `empty()`: Checks if the span is empty.
    -   `operator[]`: Accesses elements (bounds-checked if compiler/library options enable it,
        otherwise behavior is like raw array access for performance).
    -   `front()`, `back()`: Access first/last element.
    -   `data()`: Returns a `T*` to the beginning of the sequence.
    -   `begin()`, `end()`: Provide iterators, making spans compatible with range-based
        for loops and STL algorithms.
    -   Subspan creation:
        - `first(count)`: Returns a span of the first `count` elements.
        - `last(count)`: Returns a span of the last `count` elements.
        - `subspan(offset, count_opt)`: Returns a subspan. If `count_opt` is omitted
          or `std::dynamic_extent`, it goes to the end.

5.  Construction:
    -   From a C-style array: `std::span<int> s(my_array);` (size deduced)
    -   From a pointer and size: `std::span<int> s(ptr, count);`
    -   From STL containers like `std::vector`, `std::array`: `std::span<int> s(my_vector);`
    -   From iterators (if they are contiguous): `std::span<int> s(vec.begin(), vec.end());` (less common, direct container is preferred)

6.  Safety:
    -   More type-safe than passing raw pointers and sizes.
    -   Can prevent some buffer overruns if used correctly, as size information is part of the span.
    -   `operator[]` is not guaranteed to be bounds-checked by the standard for performance reasons,
        but implementations might offer checked versions (e.g., via `at()` or debug modes).

7.  Byte Access:
    -   `std::as_bytes(span_of_T)`: Returns a `std::span<const std::byte>`.
    -   `std::as_writable_bytes(span_of_T)`: Returns a `std::span<std::byte>` (for non-const T).

Use Cases:
-   Writing functions that operate on contiguous sequences of data without caring
    about the underlying container type (e.g., a function that processes data from
    either a `std::vector`, `std::array`, or C-array). This avoids code duplication
    or excessive templating.
-   Passing parts of an array/vector to functions (e.g., a slice).
-   Interfacing with C APIs that expect pointer and length.

`std::span` vs. `std::string_view`:
-   `std::string_view` (C++17): Specifically for character sequences (strings). Non-owning.
-   `std::span<T>`: Generic for any type `T`. Non-owning.
-   You can have a `std::span<const char>` which is similar to `std::string_view` but
    might have slightly different API nuances. `std::string_view` has more string-specific methods.

How to compile:
g++ -std=c++20 std_span.cpp -o std_span_example
(or clang++ -std=c++20)
Ensure your compiler and standard library fully support C++20 `<span>`.
*/
