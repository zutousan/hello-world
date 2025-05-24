// std_optional.cpp
#include <iostream>
#include <optional> // Key header for std::optional
#include <string>
#include <vector>

// Function that might return a value or nothing
std::optional<std::string> find_user_greeting(int user_id) {
    if (user_id == 1) {
        return "Hello, Alice!"; 
    } else if (user_id == 2) {
        return std::string("Welcome, Bob!"); 
    } else if (user_id == 3) {
        return {}; 
    }
    return std::nullopt; 
}

// Function that uses an optional parameter
void process_optional_config(const std::optional<std::string>& config) {
    std::cout << "Processing config: ";
    if (config.has_value()) { 
        std::cout << "\"" << config.value() << "\"" << std::endl; // Corrected quotes
    } else {
        std::cout << "[No config provided, using defaults]" << std::endl;
    }
}

struct UserData {
    std::string name;
    std::optional<int> age;
    std::optional<std::string> email;

    UserData(std::string n) : name(std::move(n)) {}
};

int main() {
    std::cout << "--- std::optional (C++17) ---" << std::endl;

    // 1. Creating optionals
    std::cout << "\n1. Creating optionals:" << std::endl; // Corrected newline formatting
    std::optional<int> opt_int1;                     
    std::optional<int> opt_int2 = std::nullopt;      
    std::optional<int> opt_int3 = 10;                
    std::optional<std::string> opt_str1 = "Hello";   
    std::optional<std::vector<int>> opt_vec = std::vector<int>{1,2,3}; 

    std::optional<std::string> opt_str_inplace(std::in_place, 5, 'A'); 
    std::cout << "opt_str_inplace: ";
    if (opt_str_inplace) std::cout << "\"" << *opt_str_inplace << "\"" << std::endl; // Corrected quotes


    // 2. Checking if an optional contains a value
    std::cout << "\n2. Checking for values:" << std::endl; // Corrected newline formatting
    std::cout << "opt_int1 has value? " << std::boolalpha << opt_int1.has_value() << std::endl;
    if (!opt_int2) { 
        std::cout << "opt_int2 is empty (checked via operator bool)." << std::endl;
    }
    if (opt_int3.has_value()) {
        std::cout << "opt_int3 has a value." << std::endl;
    }

    // 3. Accessing the contained value
    std::cout << "\n3. Accessing values:" << std::endl; // Corrected newline formatting
    if (opt_int3) {
        std::cout << "Value in opt_int3 (operator*): " << *opt_int3 << std::endl;
        std::cout << "Value in opt_int3 (value()): " << opt_int3.value() << std::endl;
    }

    try {
        std::cout << "Trying to access opt_int1.value(): ";
        int val = opt_int1.value(); 
        std::cout << val << std::endl;
    } catch (const std::bad_optional_access& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    // 4. Using value_or to provide a default
    std::cout << "\n4. Using value_or:" << std::endl; // Corrected newline formatting
    std::cout << "opt_int1.value_or(99): " << opt_int1.value_or(99) << std::endl;
    std::cout << "opt_int3.value_or(99): " << opt_int3.value_or(99) << std::endl;
    std::cout << "find_user_greeting(4).value_or(\"Default Guest\"): \"" // Corrected quotes
              << find_user_greeting(4).value_or("Default Guest") << "\"" << std::endl; // Corrected quotes

    // 5. Using optional return values from functions
    std::cout << "\n5. Optional return values:" << std::endl; // Corrected newline formatting
    for (int id : {1, 2, 3, 4}) {
        std::optional<std::string> greeting = find_user_greeting(id);
        std::cout << "For ID " << id << ": ";
        if (greeting) {
            std::cout << "\"" << *greeting << "\"" << std::endl; // Corrected quotes
        } else {
            std::cout << "[No greeting found]" << std::endl;
        }
    }

    // 6. Using optional parameters
    std::cout << "\n6. Optional parameters:" << std::endl; // Corrected newline formatting
    process_optional_config("Specific Setting");
    process_optional_config(std::nullopt);
    process_optional_config({}); 

    // 7. Modifying the contained value
    std::cout << "\n7. Modifying values:" << std::endl; // Corrected newline formatting
    if (opt_int3) {
        *opt_int3 = 20;
        std::cout << "Modified opt_int3: " << opt_int3.value() << std::endl;
    }
    opt_int3.emplace(30); 
    std::cout << "Emplaced new value in opt_int3: " << *opt_int3 << std::endl;

    // 8. reset() and assignment to std::nullopt
    std::cout << "\n8. Resetting optionals:" << std::endl; // Corrected newline formatting
    std::cout << "opt_str1 before reset: \"" << opt_str1.value_or("N/A") << "\"" << std::endl; // Corrected quotes
    opt_str1.reset(); 
    std::cout << "opt_str1 has value after reset? " << opt_str1.has_value() << std::endl;

    opt_int3 = std::nullopt; 
    std::cout << "opt_int3 has value after assigning nullopt? " << opt_int3.has_value() << std::endl;

    // 9. Optional as a class member
    std::cout << "\n9. Optional as class member:" << std::endl; // Corrected newline formatting
    UserData user1("Charlie");
    user1.age = 30;

    std::cout << "User: " << user1.name;
    if (user1.age) {
        std::cout << ", Age: " << *user1.age;
    }
    if (user1.email) {
        std::cout << ", Email: " << *user1.email;
    } else {
        std::cout << ", Email: [not provided]";
    }
    std::cout << std::endl;

    return 0;
}

/*
Explanation:
`std::optional<T>` (from the `<optional>` header) is a C++17 feature that
represents an optional value. An `std::optional<T>` object can either contain
a value of type `T`, or it can contain no value (be "empty").

It's a type-safe way to handle situations where a value might be legitimately
absent, without resorting to:
-   Magic values (e.g., -1 for an int, nullptr for pointers that don't own memory).
-   Boolean flags alongside the value.
-   Pointers that might be null (if the value isn't dynamically allocated).

Key Features:

1.  State:
    -   Either contains a `T` or is empty.
    -   `std::nullopt` is a special constant used to represent an empty `optional`.

2.  Creation:
    -   `std::optional<T> opt;` (empty)
    -   `std::optional<T> opt = std::nullopt;` (empty)
    -   `std::optional<T> opt = value_of_type_T;` (contains value)
    -   `std::optional<T> opt(std::in_place, args...);` (constructs T in-place)
    -   `std::make_optional<T>(args...)` (constructs T and returns optional, C++17)

3.  Checking for a Value:
    -   `opt.has_value()`: Returns `true` if it contains a value, `false` otherwise.
    -   `if (opt)`: The `operator bool()` is overloaded to be `true` if it contains
        a value, `false` otherwise.

4.  Accessing the Value:
    -   `*opt` (dereference operator): Returns a reference to the contained value.
        Undefined behavior if `opt` is empty.
    -   `opt.operator->()`: Accesses members of the contained value (if T is a class/struct).
        Undefined behavior if `opt` is empty.
    -   `opt.value()`: Returns a reference to the contained value. Throws
        `std::bad_optional_access` if `opt` is empty. This is the safest access
        method if you expect a value to be present.
    -   `opt.value_or(default_value)`: Returns the contained value if present,
        otherwise returns `default_value`.

5.  Modifying:
    -   `opt = new_value;`
    -   `opt.emplace(args...)`: Constructs a new value in-place inside the optional,
        replacing any existing value.
    -   `opt.reset()`: Destroys the contained value (if any) and makes the optional empty.
    -   `opt = std::nullopt;`: Makes the optional empty.

Use Cases:
-   Return values for functions that might not always produce a result (e.g.,
    searching for an item that might not be found).
-   Optional data members in classes/structs.
-   Optional function parameters.
-   Replacing uses of nullable pointers where ownership is not involved.

Benefits:
-   Expressiveness: Clearly communicates that a value might be absent.
-   Type Safety: Avoids issues with magic numbers or ambiguous null pointers.
-   Prevents Null Pointer Dereferences: Access methods like `.value()` or checks
    like `has_value()` encourage safer handling of potentially missing values.

How to compile:
g++ -std=c++17 std_optional.cpp -o std_optional_example
./std_optional_example
*/
