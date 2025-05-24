// structured_bindings.cpp
#include <iostream>
#include <tuple>
#include <string>
#include <map>
#include <utility> // For std::pair

struct MyStruct {
    int id;
    std::string name;
    double value;
};

std::tuple<int, std::string, double> get_data_tuple() {
    return {101, "TupleData", 123.45};
}

MyStruct get_data_struct() {
    return {202, "StructData", 678.90};
}

int main() {
    std::cout << "--- Structured Bindings (C++17) ---" << std::endl;

    // 1. Binding to a std::pair
    std::cout << "\n--- Binding to std::pair ---" << std::endl; // Corrected newline
    std::pair<int, std::string> myPair = {1, "Hello Pair"};
    auto [pair_id, pair_name] = myPair; // Structured binding declaration

    std::cout << "Pair ID: " << pair_id << ", Pair Name: \"" << pair_name << "\"" << std::endl; // Corrected quotes
    pair_id = 2; 
    std::cout << "Modified pair_id: " << pair_id << ", Original myPair.first: " << myPair.first << std::endl;

    const auto& [const_ref_pair_id, const_ref_pair_name] = myPair;
    std::cout << "Const Ref Pair ID: " << const_ref_pair_id << ", Const Ref Pair Name: \"" << const_ref_pair_name << "\"" << std::endl; // Corrected quotes
    // const_ref_pair_id = 3; // Error: const_ref_pair_id is const

    auto& [ref_pair_id, ref_pair_name] = myPair;
    ref_pair_id = 5; 
    std::cout << "Modified myPair.first via ref_pair_id: " << myPair.first << std::endl;


    // 2. Binding to a std::tuple
    std::cout << "\n--- Binding to std::tuple ---" << std::endl; // Corrected newline
    std::tuple<char, double, std::string> myTuple = {'A', 3.14, "Hello Tuple"};
    auto [char_val, double_val, string_val] = myTuple;

    std::cout << "Tuple char: " << char_val << ", double: " << double_val << ", string: \"" << string_val << "\"" << std::endl; // Corrected quotes

    auto [tuple_id_func, tuple_name_func, tuple_value_func] = get_data_tuple();
    std::cout << "From get_data_tuple(): ID=" << tuple_id_func << ", Name=\"" << tuple_name_func << "\", Value=" << tuple_value_func << std::endl; // Corrected quotes


    // 3. Binding to a struct or class (public data members)
    std::cout << "\n--- Binding to a struct/class ---" << std::endl; // Corrected newline
    MyStruct myS = {42, "Test Struct", 99.99};
    auto [struct_id, struct_name, struct_value] = myS; 

    std::cout << "Struct ID: " << struct_id << ", Name: \"" << struct_name << "\", Value: " << struct_value << std::endl; // Corrected quotes
    struct_id = 43; 
    std::cout << "Modified struct_id: " << struct_id << ", Original myS.id: " << myS.id << std::endl;

    auto [s_id_f, s_name_f, s_val_f] = get_data_struct();
    std::cout << "From get_data_struct(): ID=" << s_id_f << ", Name=\"" << s_name_f << "\", Value=" << s_val_f << std::endl; // Corrected quotes
    
    auto& [ref_struct_id, ref_struct_name, ref_struct_value] = myS;
    ref_struct_id = 77;
    std::cout << "Modified myS.id via ref_struct_id: " << myS.id << std::endl;


    // 4. Binding to a C-style array
    std::cout << "\n--- Binding to a C-style array ---" << std::endl; // Corrected newline
    int arr[3] = {10, 20, 30};
    auto [arr_a, arr_b, arr_c] = arr; 

    std::cout << "Array elements: a=" << arr_a << ", b=" << arr_b << ", c=" << arr_c << std::endl;
    arr_a = 11; 
    std::cout << "Modified arr_a: " << arr_a << ", Original arr[0]: " << arr[0] << std::endl;

    auto& [ref_arr_a, ref_arr_b, ref_arr_c] = arr;
    ref_arr_a = 111;
    std::cout << "Modified arr[0] via ref_arr_a: " << arr[0] << std::endl;


    // 5. Use in range-based for loops (especially with maps)
    std::cout << "\n--- Use in range-based for loops (maps) ---" << std::endl; // Corrected newline
    std::map<std::string, int> city_populations = {
        {"New York", 8398748},
        {"Los Angeles", 3990456},
        {"Chicago", 2705994}
    };

    std::cout << "City populations:" << std::endl;
    for (const auto& [city, population] : city_populations) {
        std::cout << "  " << city << ": " << population << std::endl;
    }
    
    std::cout << "Modifying populations (value part):" << std::endl;
    for (auto& [city, population] : city_populations) {
        population += 100; 
        std::cout << "  " << city << " (updated): " << population << std::endl;
    }

    return 0;
}

/*
Explanation:
Structured Bindings, introduced in C++17, provide a convenient way to unpack
values from tuples, pairs, structs, and arrays into named variables. This can
make code cleaner and more readable by avoiding manual access through `std::get`,
`.first`, `.second`, or member names repeatedly.

Syntax:
`auto [name1, name2, ..., nameN] = expression;`
`auto& [name1, name2, ..., nameN] = expression;`
`const auto& [name1, name2, ..., nameN] = expression;`
`auto&& [name1, name2, ..., nameN] = expression;` (less common, for forwarding)

- `auto` (or `auto&`, `const auto&`, etc.) is required.
- `name1, name2, ...` are the new variable names you are declaring.
- `expression` is an object that can be decomposed (a pair, tuple, struct, or array).

How it works:

1.  `std::tuple` and `std::pair`:
    -   The elements of the tuple/pair are bound to the names in order.
    -   Example: `std::tuple<int, double> t = {1, 2.5}; auto [i, d] = t;`
        `i` gets `std::get<0>(t)`, `d` gets `std::get<1>(t)`.

2.  Structs and Classes (Aggregates):
    -   The public non-static data members are bound to the names in the order
        of their declaration in the struct/class.
    -   This works for aggregate types (plain structs/classes with no user-provided
        constructors, no private/protected non-static data members, no virtual
        functions, no base classes). For non-aggregate types, they need to provide
        a tuple-like interface (specializing `std::tuple_size` and `std::get<N>`).
    -   Example: `struct Point { int x, y; }; Point p = {10,20}; auto [px, py] = p;`
        `px` gets `p.x`, `py` gets `p.y`.

3.  C-style Arrays:
    -   The elements of the array are bound to the names in order.
    -   The number of names must match the array size.
    -   Example: `int arr[] = {1,2,3}; auto [a,b,c] = arr;`
        `a` gets `arr[0]`, `b` gets `arr[1]`, `c` gets `arr[2]`.

Modifiers (`&`, `const&`):
-   `auto [x, y] = expr;` creates copies (or moves, if applicable for the "hidden" object created).
    Modifying `x` or `y` does not affect the original object in `expr` unless `expr` itself is a reference type that the hidden object refers to.
-   `auto& [x, y] = expr;` makes `x` and `y` references to the components of `expr`.
    Modifying `x` or `y` *will* affect the original object.
-   `const auto& [x, y] = expr;` makes `x` and `y` const references.

Use in Range-Based For Loops:
-   Structured bindings are particularly useful with range-based for loops,
    especially when iterating over maps, where elements are `std::pair`.
    ```cpp
    std::map<std::string, int> myMap;
    // ... populate map ...
    for (const auto& [key, value] : myMap) {
        // use key and value
    }
    ```

Benefits:
-   Readability: Improves code clarity by giving meaningful names to components.
-   Conciseness: Reduces boilerplate code for accessing elements (e.g., no
    need for multiple `std::get` calls).
-   Convenience: Simplifies working with tuple-like objects.

How to compile:
g++ -std=c++17 structured_bindings.cpp -o structured_bindings_example
./structured_bindings_example
*/
