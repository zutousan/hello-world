// std_tuple.cpp
#include <iostream>
#include <tuple>    // For std::tuple
#include <string>
#include <vector>
#include <utility>  // For std::index_sequence (typically C++14, but needed for the print helper)

// Function that returns multiple values using a tuple
std::tuple<int, std::string, double> get_student_info(int student_id) {
    if (student_id == 1) {
        return std::make_tuple(20, "Alice Smith", 3.8);
    } else if (student_id == 2) {
        return std::make_tuple(22, "Bob Johnson", 3.5);
    } else {
        return std::make_tuple(0, "Unknown", 0.0);
    }
}

// Helper function to print a tuple (C++11 style, a bit verbose)
template<typename TupleType, size_t... I>
void print_tuple_impl(const TupleType& t, std::index_sequence<I...>) {
    std::cout << "(";
    // C++17 fold expression would be cleaner: ((std::cout << (I == 0 ? "" : ", ") << std::get<I>(t)), ...);
    // C++11 way:
    size_t index = 0;
    auto print_element = [&](const auto& x) {
        if (index++ > 0) std::cout << ", ";
        std::cout << x;
    };
    // Use an initializer list to expand the parameter pack and call the lambda
    // This is a common C++11/14 trick for pack expansion in a specific order.
    (void)std::initializer_list<int>{(print_element(std::get<I>(t)), 0)...};
    std::cout << ")" << std::endl;
}

template<typename... Args>
void print_tuple(const std::tuple<Args...>& t) {
    print_tuple_impl(t, std::index_sequence_for<Args...>{});
}


int main() {
    // --- 1. Creating Tuples ---
    std::cout << "--- Creating Tuples ---" << std::endl;

    // a. Using std::make_tuple (type deduction)
    auto t1 = std::make_tuple(10, std::string("Hello"), 3.14, true);
    std::cout << "t1 created with make_tuple: ";
    print_tuple(t1);

    // b. Explicitly specifying types
    std::tuple<int, char, std::string> t2(20, 'A', "World");
    std::cout << "t2 created with explicit types: ";
    print_tuple(t2);

    // c. Empty tuple
    std::tuple<> empty_tuple;
    std::cout << "empty_tuple: ";
    print_tuple(empty_tuple);


    // --- 2. Accessing Tuple Elements ---
    std::cout << "\n--- Accessing Tuple Elements ---" << std::endl;
    // Use std::get<index>(tuple_object)
    // Index must be a compile-time constant.

    std::cout << "Element 0 of t1 (int): " << std::get<0>(t1) << std::endl;
    std::cout << "Element 1 of t1 (string): " << std::get<1>(t1) << std::endl;
    std::cout << "Element 2 of t1 (double): " << std::get<2>(t1) << std::endl;
    std::cout << "Element 3 of t1 (bool): " << std::boolalpha << std::get<3>(t1) << std::endl;

    // Modifying an element
    std::get<0>(t1) = 100;
    std::cout << "Modified t1: ";
    print_tuple(t1);

    // --- 3. Getting Tuple Size and Type Info (Compile Time) ---
    std::cout << "\n--- Tuple Size and Type Info ---" << std::endl;
    std::cout << "Size of t1 (number of elements): " << std::tuple_size<decltype(t1)>::value << std::endl;

    // Get type of an element
    std::tuple_element<1, decltype(t1)>::type second_element_t1 = std::get<1>(t1);
    std::cout << "Type of t1's second element is std::string, value: " << second_element_t1 << std::endl;


    // --- 4. Unpacking Tuples (Structured Bindings - C++17) ---
    std::cout << "\n--- Unpacking Tuples ---" << std::endl;
    // In C++17, structured bindings make this much cleaner:
    // auto [age_s, name_s, gpa_s] = get_student_info(1);
    // std::cout << "C++17 style unpack: Name: " << name_s << ", Age: " << age_s << ", GPA: " << gpa_s << std::endl;
    std::cout << "(C++17 structured bindings like 'auto [a,b,c] = mytuple;' are not used in this C++11 example.)" << std::endl;

    // C++11 way to "unpack" using std::tie
    // std::tie creates a tuple of lvalue references.
    int age;
    std::string name;
    double gpa;

    // Method 1: Assign from a tuple returned by a function
    std::tie(age, name, gpa) = get_student_info(1);
    std::cout << "Unpacked (get_student_info(1)) with std::tie: Age=" << age << ", Name='" << name << "', GPA=" << gpa << std::endl;

    // Method 2: Assign from an existing tuple
    std::tie(age, name, gpa) = t1; // Assuming t1 has compatible types for the first 3 elements
                                   // This will only assign the first 3 elements of t1 to age, name, gpa
    std::cout << "Unpacked (t1) with std::tie: Age=" << age << ", Name='" << name << "', GPA=" << gpa << std::endl;
    // Note: std::get<3>(t1) (the bool) is ignored by this std::tie call.

    // std::ignore can be used to skip certain elements during unpacking
    double gpa_only;
    std::tie(std::ignore, std::ignore, gpa_only) = get_student_info(2);
    std::cout << "Unpacked (get_student_info(2)) with std::tie and std::ignore: GPA=" << gpa_only << std::endl;


    // --- 5. Tuple Comparison ---
    // Tuples are compared lexicographically (element by element).
    std::cout << "\n--- Tuple Comparison ---" << std::endl;
    auto tuple_a = std::make_tuple(1, std::string("apple"));
    auto tuple_b = std::make_tuple(1, std::string("banana"));
    auto tuple_c = std::make_tuple(2, std::string("apple"));
    auto tuple_d = std::make_tuple(1, std::string("apple"));

    std::cout << "tuple_a: "; print_tuple(tuple_a);
    std::cout << "tuple_b: "; print_tuple(tuple_b);
    std::cout << "tuple_c: "; print_tuple(tuple_c);
    std::cout << "tuple_d: "; print_tuple(tuple_d);

    std::cout << std::boolalpha;
    std::cout << "tuple_a == tuple_d: " << (tuple_a == tuple_d) << std::endl; // true
    std::cout << "tuple_a != tuple_b: " << (tuple_a != tuple_b) << std::endl; // true
    std::cout << "tuple_a < tuple_b: " << (tuple_a < tuple_b) << std::endl;   // true (apple < banana)
    std::cout << "tuple_b < tuple_c: " << (tuple_b < tuple_c) << std::endl;   // true (1 < 2)


    // --- 6. Concatenating Tuples (std::tuple_cat) ---
    std::cout << "\n--- Tuple Concatenation ---" << std::endl;
    auto cat1 = std::make_tuple(1, "one");
    auto cat2 = std::make_tuple(2.0, "two", true);
    auto concatenated_tuple = std::tuple_cat(cat1, empty_tuple, cat2);

    std::cout << "cat1: "; print_tuple(cat1);
    std::cout << "cat2: "; print_tuple(cat2);
    std::cout << "concatenated_tuple: "; print_tuple(concatenated_tuple);
    // Expected: (1, one, 2.0, two, true)

    return 0;
}

/*
Explanation:
`std::tuple` (from the `<tuple>` header) is a C++11 feature that represents a
fixed-size collection of heterogeneous values. It's a generalization of
`std::pair`. Each element in a tuple can be of a different type.

Key Features and Usage:

1.  Creation:
    -   `std::make_tuple(args...)`: Helper function that deduces the types of the
        elements and creates a tuple. (Preferred way)
        `auto t = std::make_tuple(10, "hello", 3.14);`
    -   Explicitly: `std::tuple<int, std::string, double> t(10, "hello", 3.14);`

2.  Accessing Elements:
    -   `std::get<N>(my_tuple)`: Accesses the N-th element (0-indexed).
        `N` must be a compile-time constant integer.
        `int i = std::get<0>(t);`
    -   If you try to access an element out of bounds (at compile time), it's a
        compile error.

3.  Tuple Size and Element Types (Compile-Time):
    -   `std::tuple_size<decltype(my_tuple)>::value`: Gets the number of elements.
    -   `std::tuple_element<N, decltype(my_tuple)>::type`: Gets the type of the
        N-th element.

4.  Unpacking (Assigning to Variables):
    -   `std::tie(var1, var2, ...)`: Creates a tuple of lvalue references to
        `var1, var2, ...`. This can be assigned from another tuple to unpack its
        values into the variables.
        `std::tie(i, s, d) = my_tuple;`
    -   `std::ignore`: Can be used as a placeholder in `std::tie` if you want to
        skip some elements during unpacking.
        `std::tie(i, std::ignore, d) = my_tuple;`
    -   C++17 introduces structured bindings, which provide a much cleaner syntax
        for unpacking: `auto [i, s, d] = my_tuple;`

5.  Comparison:
    -   Tuples can be compared using relational operators (`==`, `!=`, `<`, `>`, `<=`, `>=`).
    -   Comparison is lexicographical: elements are compared one by one from left
        to right until a difference is found or all elements are equal.

6.  `std::tuple_cat(tuple1, tuple2, ...)`:
    -   Concatenates multiple tuples into a single new tuple.

Use Cases:
-   Returning multiple values from a function.
-   Storing a fixed-size collection of heterogeneous items.
-   Can be used as keys in associative containers like `std::map` if all its
    element types support the required comparison operators.

Example: Printing a tuple generically in C++11/14 often involves index sequences
(`std::index_sequence`, `std::make_index_sequence`, `std::index_sequence_for`)
to expand operations over tuple elements, as direct iteration is not possible.
The `print_tuple` helper in this example demonstrates this technique.

How to compile:
g++ -std=c++11 std_tuple.cpp -o std_tuple_example
./std_tuple_example
*/
