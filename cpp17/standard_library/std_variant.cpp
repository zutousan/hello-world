// std_variant.cpp
#include <iostream>
#include <variant>  // Key header for std::variant
#include <string>
#include <vector>
#include <iomanip>  // For std::fixed, std::setprecision

// Visitor struct for std::visit
struct MyVisitor {
    void operator()(int i) const {
        std::cout << "Visited int: " << i << std::endl;
    }
    void operator()(double d) const {
        std::cout << "Visited double: " << std::fixed << std::setprecision(2) << d << std::endl;
    }
    void operator()(const std::string& s) const {
        std::cout << "Visited string: \"" << s << "\"" << std::endl;
    }
    // Catch-all for types not explicitly handled (less common to need if all types are covered)
    template<typename T>
    void operator()(const T& other) const {
        std::cout << "Visited an unhandled type in MyVisitor." << std::endl;
    }
};

// Another visitor example using lambdas with std::overload (C++20 pattern, but can be simulated)
// For C++17, you'd typically use a struct like MyVisitor or separate if/get calls.
// To illustrate the concept for C++17, we'll stick to the struct visitor or direct checks.

int main() {
    std::cout << "--- std::variant (C++17) ---" << std::endl;

    // 1. Creating variants
    std::cout << "\n1. Creating variants:" << std::endl;
    std::variant<int, double, std::string> var1; // Default-constructs to the first type (int), value-initialized (0)
    std::cout << "var1 (default int): " << std::get<int>(var1) << std::endl;

    var1 = 123;       // Assigns int, now holds 123
    std::cout << "var1 (assigned int): " << std::get<int>(var1) << std::endl;

    var1 = 3.14159;   // Assigns double, now holds 3.14159
    std::cout << "var1 (assigned double): " << std::get<double>(var1) << std::endl;

    var1 = "Hello Variant!"; // Assigns string
    std::cout << "var1 (assigned string): \"" << std::get<std::string>(var1) << "\"" << std::endl;

    std::variant<int, std::string> var2 = "Test"; // Holds "Test"
    // std::variant<int, double> var3 = "error"; // Compile error: "error" not convertible to int or double

    // In-place construction
    std::variant<int, std::string, std::vector<int>> var_complex;
    var_complex.emplace<std::vector<int>>({1, 2, 3, 4, 5});
    std::cout << "var_complex (vector): size " << std::get<std::vector<int>>(var_complex).size() << std::endl;


    // 2. Checking the active type: index() and std::holds_alternative
    std::cout << "\n2. Checking active type:" << std::endl;
    std::cout << "var1 current index: " << var1.index(); // 0 for int, 1 for double, 2 for string
    if (var1.index() == 2) { // index for std::string
        std::cout << " (which is std::string)" << std::endl;
    }

    if (std::holds_alternative<std::string>(var1)) {
        std::cout << "var1 currently holds a std::string." << std::endl;
    } else if (std::holds_alternative<double>(var1)) {
        std::cout << "var1 currently holds a double." << std::endl;
    }


    // 3. Accessing the value: std::get<index> or std::get<Type>
    std::cout << "\n3. Accessing values:" << std::endl;
    if (std::holds_alternative<std::string>(var1)) {
        std::cout << "Value via std::get<std::string>: \"" << std::get<std::string>(var1) << "\"" << std::endl;
    }
    // Access by index (if you know it)
    // std::cout << "Value via std::get<2>: " << std::get<2>(var1) << std::endl; // Assuming index 2 is string

    // std::get throws std::bad_variant_access if the type/index is wrong
    try {
        std::cout << "Trying std::get<int>(var1): ";
        int val = std::get<int>(var1); // var1 holds string, this throws
        std::cout << val << std::endl;
    } catch (const std::bad_variant_access& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    // std::get_if<Type/index> returns a pointer or nullptr (no exception)
    std::cout << "Using std::get_if:" << std::endl;
    if (auto* str_ptr = std::get_if<std::string>(&var1)) { // Note: takes pointer to variant
        std::cout << "  Found string via get_if: \"" << *str_ptr << "\"" << std::endl;
        *str_ptr += " (modified)"; // Can modify if variant is not const
    } else if (auto* dbl_ptr = std::get_if<double>(&var1)) {
        std::cout << "  Found double via get_if: " << *dbl_ptr << std::endl;
    }
    // Check modification
    if (auto* str_ptr = std::get_if<std::string>(&var1)) {
        std::cout << "  After modification attempt: \"" << *str_ptr << "\"" << std::endl;
    }


    // 4. Using std::visit to apply a visitor
    std::cout << "\n4. Using std::visit:" << std::endl;
    var1 = 42;
    std::visit(MyVisitor{}, var1); // Pass visitor object and variant

    var1 = "Another visit";
    std::visit(MyVisitor{}, var1);

    var1 = 123.456;
    std::visit(MyVisitor{}, var1);
    
    // Visiting multiple variants (all visitors must handle all type combinations)
    // std::variant<int, float> v_a = 10;
    // std::variant<long, double> v_b = 20.0;
    // std::visit([](auto val_a, auto val_b){ /* ... */ }, v_a, v_b); // More complex


    // 5. Special states
    std::cout << "\n5. Special states:" << std::endl;
    // `valueless_by_exception()`: true if variant is in an error state
    // (e.g., an exception was thrown during assignment/emplace and it couldn't recover)
    // This is rare in normal usage unless types have throwing move/copy constructors.
    std::cout << "var1 valueless_by_exception? " << std::boolalpha << var1.valueless_by_exception() << std::endl;

    // Default constructed variant holds the first type, value-initialized.
    std::variant<std::string, int> var_default_str_first; // Holds "" (empty string)
    std::cout << "var_default_str_first holds: \"" << std::get<std::string>(var_default_str_first) << "\"" << std::endl;


    std::cout << "\nVariant example finished." << std::endl;
    return 0;
}

/*
Explanation:
`std::variant<Types...>` (from the `<variant>` header) is a C++17 feature that
represents a type-safe union. An instance of `std::variant` can hold a value
of one of its specified alternative types at any given time.

Unlike C-style unions, `std::variant` is type-safe:
-   It always knows which type it currently holds.
-   Accessing it as the wrong type results in a well-defined error (exception or nullptr).

Key Features:

1.  Declaration and Initialization:
    -   `std::variant<int, double, std::string> v;`
        (Defaults to holding the first type, `int`, which is value-initialized to 0).
    -   `v = 10;` (Now holds an `int`)
    -   `v = "hello";` (Now holds a `std::string`)
    -   `v.emplace<NewType>(args...);` (Constructs `NewType` in-place).

2.  Querying the Active Type:
    -   `v.index()`: Returns the 0-based index of the type currently held.
    -   `std::holds_alternative<Type>(v)`: Returns `true` if `v` currently holds `Type`.

3.  Accessing the Value:
    -   `std::get<Type>(v)` or `std::get<index>(v)`: Returns a reference to the
        value. Throws `std::bad_variant_access` if `v` does not hold `Type` or
        if `index` is incorrect.
    -   `std::get_if<Type>(&v)` or `std::get_if<index>(&v)`: Returns a pointer
        to the value if `v` holds `Type`/`index`, otherwise returns `nullptr`.
        (Safer, as it doesn't throw). Note the `&v` (pointer to variant).

4.  `std::visit`:
    -   The most powerful way to work with variants. It applies a "Visitor"
        callable object to the value currently held by the variant.
    -   The Visitor must be able to handle all possible types the variant can hold
        (e.g., by providing overloaded `operator()` for each type).
    ```cpp
    struct MyVisitor {
        void operator()(int i) { ... }
        void operator()(const std::string& s) { ... }
    };
    std::visit(MyVisitor{}, my_variant);
    ```
    -   Lambdas with `if constexpr` or an "overloaded lambda" pattern (more common with C++20 `std::overload`)
        can also be used as visitors.

5.  `valueless_by_exception`:
    -   A `std::variant` can become "valueless" if an exception is thrown during
        an operation that changes its state (e.g., assignment or emplace), and
        the old value cannot be restored. This is a rare state.
    -   `v.valueless_by_exception()` checks for this.

Use Cases:
-   Representing sum types (a value can be one of several distinct types).
-   Implementing state machines where states have different associated data.
-   Parsing data that can result in different types (e.g., JSON values).
-   Error handling (e.g., `std::variant<ResultType, ErrorType>`).

`std::variant` vs. `std::any` vs. `std::optional`:
-   `std::optional<T>`: Represents a value that might be `T` or empty. Fixed type `T`.
-   `std::variant<T1, T2, ...>`: Holds one value from a *closed set* of specified types.
    The set of possible types is known at compile time.
-   `std::any` (C++17): Can hold a value of (almost) *any* type. The type is
    determined at runtime. Less type-safe at compile time than `std::variant`.

How to compile:
g++ -std=c++17 std_variant.cpp -o std_variant_example
./std_variant_example
*/
