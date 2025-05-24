// ranges.cpp
#include <iostream>
#include <vector>
#include <ranges>     // Key header for ranges and views
#include <string>
#include <algorithm>  // For std::sort (though ranges has its own sort)
#include <map>        // For map examples

// Helper to print elements of most ranges/views
template<std::ranges::range R> // Constrain R to be a range
void print_range(const R& r, const std::string& title = "") {
    if (!title.empty()) {
        std::cout << title << ": ";
    }
    std::cout << "[";
    bool first = true;
    for (const auto& elem : r) {
        if (!first) {
            std::cout << ", ";
        }
        // Check if element is printable directly, otherwise indicate type
        if constexpr (requires{ std::cout << elem; }) {
            std::cout << elem;
        } else if constexpr (std::is_same_v<decltype(elem), const std::pair<const std::string, int>&>) { // Specific for map element
             std::cout << "{"" << elem.first << "": " << elem.second << "}";
        }
        else {
            std::cout << "<" << typeid(elem).name() << ">";
        }
        first = false;
    }
    std::cout << "]" << std::endl;
}


int main() {
    std::cout << "--- C++20 Ranges ---" << std::endl;

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<std::string> words = {"apple", "banana", "cherry", "date", "apricot", "blueberry"};
    std::map<std::string, int> fruit_calories = {
        {"apple", 95}, {"banana", 105}, {"cherry", 50}, {"date", 282}
    };

    // --- 1. Basic Views: filter and transform ---
    std::cout << "\n1. Basic Views: filter and transform" << std::endl;

    // Filter even numbers
    auto even_numbers_view = numbers | std::views::filter([](int n){ return n % 2 == 0; });
    print_range(even_numbers_view, "Even numbers");

    // Transform: square each even number
    // Note: views are lazy and composable. The operations are not performed until iteration.
    auto squared_even_numbers_view = even_numbers_view | std::views::transform([](int n){ return n * n; });
    print_range(squared_even_numbers_view, "Squared even numbers");

    // Chaining multiple operations directly
    auto result_view = numbers
                     | std::views::filter([](int n){ return n > 3 && n < 8; }) // {4, 5, 6, 7}
                     | std::views::transform([](int n){ return std::to_string(n) + "s"; }); // {"4s", "5s", "6s", "7s"}
    print_range(result_view, "Numbers >3 & <8, stringified");


    // --- 2. Other common views ---
    std::cout << "\n2. Other common views" << std::endl;

    // std::views::take: take the first N elements
    auto first_three = numbers | std::views::take(3);
    print_range(first_three, "First three numbers");

    // std::views::drop: skip the first N elements
    auto skip_three = numbers | std::views::drop(3);
    print_range(skip_three, "Skip first three numbers");

    // std::views::reverse: view elements in reverse order
    auto reversed_numbers = numbers | std::views::reverse;
    print_range(reversed_numbers, "Reversed numbers (view)");
    // Note: `numbers` itself is not modified.

    // std::views::keys and std::views::values (for map-like ranges)
    print_range(fruit_calories | std::views::keys, "Fruit names (keys)");
    print_range(fruit_calories | std::views::values, "Fruit calories (values)");

    // std::views::elements<N> (for tuple-like ranges)
    std::vector<std::pair<int, std::string>> pairs = {{1, "one"}, {2, "two"}, {3, "three"}};
    print_range(pairs | std::views::elements<0>, "First elements of pairs");
    print_range(pairs | std::views::elements<1>, "Second elements of pairs");
    
    // std::views::iota (generate a sequence)
    print_range(std::views::iota(1, 6), "iota(1, 6)"); // [1, 2, 3, 4, 5]
    print_range(std::views::iota(0) | std::views::take(5), "iota(0) | take(5)"); // [0, 1, 2, 3, 4]


    // --- 3. Range Adaptor Objects vs Pipeable Views ---
    // Some range adaptors are objects that take a range as an argument,
    // others are pipeable using `|`. Most modern ones are pipeable.

    // --- 4. Range factories ---
    // `std::views::iota` is a range factory (creates a new range).
    // `std::views::single(value)` creates a range with one element.
    // `std::views::empty<T>()` creates an empty range of type T.
    print_range(std::views::single(100), "views::single(100)");
    print_range(std::views::empty<int>, "views::empty<int>");


    // --- 5. Converting views to containers (materializing) ---
    // Views are lazy. To get a container with the results:
    std::cout << "\n5. Converting views to containers" << std::endl;
    auto first_five_squared_vec = numbers
                                | std::views::take(5)
                                | std::views::transform([](int n){ return n * n; })
                                | std::ranges::to<std::vector>(); // C++23 feature, use loop for C++20
    // C++20 way to materialize:
    std::vector<int> first_five_squared_vec_cpp20;
    auto temp_view = numbers | std::views::take(5) | std::views::transform([](int n){ return n * n; });
    // std::ranges::copy(temp_view, std::back_inserter(first_five_squared_vec_cpp20)); // or a loop
    for(int val : temp_view) { first_five_squared_vec_cpp20.push_back(val); }
    print_range(first_five_squared_vec_cpp20, "Materialized vector (first 5 squared)");


    // --- 6. Range-based algorithms ---
    // C++20 also introduces versions of many algorithms that operate directly on ranges.
    std::cout << "\n6. Range-based algorithms" << std::endl;
    std::vector<int> unsorted_numbers = {5, 1, 4, 2, 8, 0, 3};
    print_range(unsorted_numbers, "Unsorted for range-sort");
    
    std::ranges::sort(unsorted_numbers); // Sorts the vector in place
    print_range(unsorted_numbers, "Sorted with std::ranges::sort");

    // Find an element
    auto it_found = std::ranges::find(unsorted_numbers, 4);
    if (it_found != unsorted_numbers.end()) {
        std::cout << "Found 4 at index: " << std::distance(unsorted_numbers.begin(), it_found) << std::endl;
    }

    // Using ranges algorithms with views
    auto even_numbers_again = numbers | std::views::filter([](int n){ return n % 2 == 0; });
    std::cout << "Using std::ranges::for_each on a view:" << std::endl;
    std::ranges::for_each(even_numbers_again, [](int n){ std::cout << "Even: " << n << std::endl; });


    // --- 7. Projections ---
    // Many range algorithms accept a "projection" argument, which is a unary function
    // applied to each element before the main operation.
    std::cout << "\n7. Projections" << std::endl;
    struct Person { std::string name; int age; };
    std::vector<Person> people = {{"Alice", 30}, {"Bob", 25}, {"Charlie", 35}};
    
    // Sort people by age using a projection
    std::ranges::sort(people, {}, &Person::age); // {} is default comparator, &Person::age is projection
    std::cout << "People sorted by age:" << std::endl;
    for (const auto& p : people) { std::cout << "  {"" << p.name << "", " << p.age << "}" << std::endl; }

    // Find person named "Bob"
    auto bob_it = std::ranges::find(people, "Bob", &Person::name);
    if (bob_it != people.end()) {
        std::cout << "Found " << bob_it->name << ", age " << bob_it->age << std::endl;
    }

    std::cout << "\nRanges make working with collections more expressive and less error-prone." << std::endl;

    return 0;
}

/*
Explanation:
C++20 Ranges are a major addition that revolutionizes how collections of data
are processed. They provide a way to work with sequences of elements in a more
composable, lazy, and expressive manner.

Key Header: `<ranges>`

Core Concepts:

1.  Range:
    -   A `range` is anything that can be iterated over (e.g., standard containers
        like `std::vector`, C-style arrays, or custom types that expose `begin()`
        and `end()` iterators, or for which `std::ranges::begin` and `std::ranges::end`
        can be found).
    -   The `std::ranges::range` concept checks if a type is a range.

2.  Views:
    -   A `view` is a lightweight, non-owning, lazy representation of a range.
    -   Views adapt or transform existing ranges without copying the underlying data.
    -   Operations on views are typically performed on-demand when the view is iterated.
    -   Views are composable using the pipe operator (`|`).
    -   Many views are provided in the `std::views` namespace (or `std::ranges::views`).
    -   Examples:
        - `std::views::filter(pred)`: A view containing elements that satisfy `pred`.
        - `std::views::transform(func)`: A view where each element is the result of `func` applied to an element of the underlying range.
        - `std::views::take(n)`: A view of the first `n` elements.
        - `std::views::drop(n)`: A view that skips the first `n` elements.
        - `std::views::reverse`: A view of elements in reverse order.
        - `std::views::keys`, `std::views::values`: For map-like ranges.
        - `std::views::elements<N>`: For tuple-like ranges.
        - `std::views::iota(start, end)`: Generates a sequence of numbers.

3.  Range Adaptors:
    -   These are functions or objects that produce views from ranges.
    -   Many are pipeable: `range | view_adaptor_1 | view_adaptor_2`
    -   Example: `numbers | std::views::filter(...) | std::views::transform(...)`

4.  Range Factories:
    -   Functions that create new ranges from scratch (not from existing ranges).
    -   Examples: `std::views::iota(0, 10)`, `std::views::single(42)`.

5.  Range Algorithms:
    -   C++20 overloads many standard algorithms (from `<algorithm>`) to work
        directly with ranges instead of iterator pairs.
    -   These are found in the `std::ranges` namespace (e.g., `std::ranges::sort`,
        `std::ranges::for_each`, `std::ranges::find`).
    -   They often accept projections.

6.  Projections:
    -   A projection is a callable object (e.g., lambda, function pointer, member pointer)
        that is applied to each element *before* the main operation of an algorithm.
    -   This allows algorithms to operate on a specific part or transformation of an element.
    -   Example: Sorting a vector of `Person` objects by their `age` member:
        `std::ranges::sort(people, {}, &Person::age);`
        Here, `{}` is the default comparator, and `&Person::age` is the projection.

Benefits of Ranges:
-   Composability: Chain multiple operations easily: `range | op1 | op2 | op3`.
-   Laziness: Views perform operations on demand, potentially avoiding unnecessary
    computation or memory allocation for intermediate results.
-   Readability: Code often becomes more declarative and easier to understand.
-   Reduced Boilerplate: No more manual iterator loops for many common tasks.
-   Safety: Less prone to iterator invalidation issues or off-by-one errors
    compared to manual iterator manipulation.
-   Efficiency: Lazy evaluation can lead to performance improvements by avoiding
    intermediate storage and enabling better optimization (e.g., loop fusion).

Materializing Views:
-   Since views are lazy, if you need the results in a concrete container, you
    must "materialize" the view.
-   In C++20, one way is to iterate and copy:
    `std::vector<T> result_vec; for(auto const& x : my_view) result_vec.push_back(x);`
    Or use `std::ranges::copy(my_view, std::back_inserter(result_vec));`
-   C++23 adds `std::ranges::to<ContainerType>()` for direct conversion:
    `auto result_vec = my_view | std::ranges::to<std::vector>();`

Ranges are a fundamental shift in how C++ programmers can approach data processing,
making it more aligned with functional programming paradigms while retaining C++'s
performance characteristics.

How to compile:
g++ -std=c++20 ranges.cpp -o ranges_example
(or clang++ -std=c++20)
Ensure your compiler and standard library fully support C++20 ranges.
*/
