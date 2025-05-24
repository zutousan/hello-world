// ranges.cpp
#include <iostream>
#include <vector>
#include <ranges>     // Key header for ranges and views
#include <string>
#include <algorithm>  // For std::sort, std::distance, std::ranges::copy
#include <map>        // For map examples
#include <type_traits> // For std::is_same_v
#include <iterator>   // For std::back_inserter

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
             // Special handling for map elements to print them nicely
            if constexpr (std::is_same_v<std::remove_cvref_t<decltype(elem)>, std::pair<const std::string, int>>) {
                 std::cout << "{\"" << elem.first << "\": " << elem.second << "}";
            } else {
                std::cout << elem;
            }
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

    auto even_numbers_view = numbers | std::views::filter([](int n){ return n % 2 == 0; });
    print_range(even_numbers_view, "Even numbers");

    auto squared_even_numbers_view = even_numbers_view | std::views::transform([](int n){ return n * n; });
    print_range(squared_even_numbers_view, "Squared even numbers");

    auto result_view = numbers
                     | std::views::filter([](int n){ return n > 3 && n < 8; }) 
                     | std::views::transform([](int n){ return std::to_string(n) + "s"; }); 
    print_range(result_view, "Numbers >3 & <8, stringified");


    // --- 2. Other common views ---
    std::cout << "\n2. Other common views" << std::endl;

    auto first_three = numbers | std::views::take(3);
    print_range(first_three, "First three numbers");

    auto skip_three = numbers | std::views::drop(3);
    print_range(skip_three, "Skip first three numbers");

    auto reversed_numbers = numbers | std::views::reverse;
    print_range(reversed_numbers, "Reversed numbers (view)");

    print_range(fruit_calories | std::views::keys, "Fruit names (keys)");
    print_range(fruit_calories | std::views::values, "Fruit calories (values)");

    std::vector<std::pair<int, std::string>> pairs = {{1, "one"}, {2, "two"}, {3, "three"}};
    print_range(pairs | std::views::elements<0>, "First elements of pairs");
    print_range(pairs | std::views::elements<1>, "Second elements of pairs");
    
    print_range(std::views::iota(1, 6), "iota(1, 6)"); 
    print_range(std::views::iota(0) | std::views::take(5), "iota(0) | take(5)"); 


    // --- 4. Range factories --- (Section 3 was just a comment, removed for brevity)
    print_range(std::views::single(100), "views::single(100)");
    print_range(std::views::empty<int>, "views::empty<int>");


    // --- 5. Converting views to containers (materializing) ---
    std::cout << "\n5. Converting views to containers" << std::endl;
    // C++20 way to materialize:
    std::vector<int> first_five_squared_vec_cpp20;
    auto temp_view_for_materialize = numbers | std::views::take(5) | std::views::transform([](int n){ return n * n; });
    // Option 1: Loop
    // for(int val : temp_view_for_materialize) { first_five_squared_vec_cpp20.push_back(val); }
    // Option 2: std::ranges::copy (requires <algorithm> and <iterator> for back_inserter)
    std::ranges::copy(temp_view_for_materialize, std::back_inserter(first_five_squared_vec_cpp20));
    print_range(first_five_squared_vec_cpp20, "Materialized vector (first 5 squared)");


    // --- 6. Range-based algorithms ---
    std::cout << "\n6. Range-based algorithms" << std::endl;
    std::vector<int> unsorted_numbers = {5, 1, 4, 2, 8, 0, 3};
    print_range(unsorted_numbers, "Unsorted for range-sort");
    
    std::ranges::sort(unsorted_numbers); 
    print_range(unsorted_numbers, "Sorted with std::ranges::sort");

    auto it_found = std::ranges::find(unsorted_numbers, 4);
    if (it_found != unsorted_numbers.end()) {
        std::cout << "Found 4 at index: " << std::distance(unsorted_numbers.begin(), it_found) << std::endl;
    }

    auto even_numbers_again = numbers | std::views::filter([](int n){ return n % 2 == 0; });
    std::cout << "Using std::ranges::for_each on a view:" << std::endl;
    std::ranges::for_each(even_numbers_again, [](int n){ std::cout << "Even: " << n << std::endl; });


    // --- 7. Projections ---
    std::cout << "\n7. Projections" << std::endl;
    struct Person { std::string name; int age; };
    std::vector<Person> people = {{"Alice", 30}, {"Bob", 25}, {"Charlie", 35}};
    
    std::ranges::sort(people, {}, &Person::age); 
    std::cout << "People sorted by age:" << std::endl;
    // Corrected quoting for Person output
    for (const auto& p : people) { std::cout << "  {\"" << p.name << "\", " << p.age << "}" << std::endl; }

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
1. Range: Anything iterable (containers, C-arrays, custom types with begin/end).
2. Views: Lightweight, non-owning, lazy representations of ranges. They adapt or
   transform existing ranges without copying data. Operations are on-demand.
   Composable using `|`. Found in `std::views`.
   Examples: `filter`, `transform`, `take`, `drop`, `reverse`, `keys`, `values`, `elements`, `iota`.
3. Range Adaptors: Produce views from ranges (often pipeable).
4. Range Factories: Create new ranges (e.g., `iota`, `single`).
5. Range Algorithms: Overloads of standard algorithms in `std::ranges` that
   operate directly on ranges and often accept projections.
6. Projections: Unary functions applied to elements *before* an algorithm's main operation.

Benefits: Composability, laziness, readability, reduced boilerplate, safety, potential efficiency.

Materializing Views (C++20):
- Since views are lazy, convert to a container if needed.
- Loop: `for(auto const& x : my_view) vec.push_back(x);`
- `std::ranges::copy(my_view, std::back_inserter(vec));`
- (C++23 adds `std::ranges::to<ContainerType>()`)

How to compile:
g++ -std=c++20 ranges.cpp -o ranges_example
(or clang++ -std=c++20)
Ensure your compiler and standard library fully support C++20 ranges.
*/
