// lambda_expressions.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 1. Basic lambda: sum of two numbers
    auto sum = [](int a, int b) -> int {
        return a + b;
    };
    std::cout << "Sum of 5 and 3: " << sum(5, 3) << std::endl;

    // 2. Lambda with capture by value: multiply each element by a factor
    int factor = 10;
    std::for_each(numbers.begin(), numbers.end(), [factor](int& n) {
        n *= factor; // n is modified, but 'factor' is a copy
    });
    std::cout << "Numbers multiplied by " << factor << " (capture by value): ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // Reset numbers
    numbers = {1, 2, 3, 4, 5};

    // 3. Lambda with capture by reference: add to each element
    int to_add = 5;
    std::for_each(numbers.begin(), numbers.end(), [&to_add](int& n) {
        n += to_add; // 'to_add' is a reference, changes to it outside affect this
    });
    std::cout << "Numbers after adding " << to_add << " (capture by reference): ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    // to_add = 10; // If 'to_add' changed here, the lambda would use the new value if called again

    // 4. Lambda with mixed capture (value and reference)
    int x = 2, y = 3;
    auto mixed_capture_lambda = [x, &y]() {
        // x is captured by value (read-only copy)
        // y is captured by reference (can be modified if lambda is mutable or y is modified outside)
        std::cout << "Inside mixed_capture_lambda: x=" << x << ", y=" << y << std::endl;
        // x = 10; // Error: cannot assign to a variable captured by copy in a non-mutable lambda
        y = 30;   // OK: y is captured by reference
    };
    mixed_capture_lambda();
    std::cout << "Outside mixed_capture_lambda: x=" << x << ", y=" << y << std::endl;


    // 5. Lambda with default capture by value and specific capture by reference
    int val_a = 1, val_b = 2, val_c = 3;
    auto default_value_capture = [=, &val_b]() { // Captures val_a by value (default), val_b by reference
        std::cout << "default_value_capture: val_a=" << val_a << ", val_b=" << val_b << ", val_c=" << val_c << std::endl;
        // val_a = 10; // Error
        val_b = 20; // OK
        // val_c = 30; // Error
    };
    default_value_capture();
    std::cout << "After default_value_capture: val_a=" << val_a << ", val_b=" << val_b << ", val_c=" << val_c << std::endl;

    // 6. Lambda with default capture by reference and specific capture by value
    int ref_a = 1, ref_b = 2, ref_c = 3;
    auto default_ref_capture = [&, ref_b]() mutable { // Captures ref_a, ref_c by reference, ref_b by value
        std::cout << "default_ref_capture: ref_a=" << ref_a << ", ref_b=" << ref_b << ", ref_c=" << ref_c << std::endl;
        ref_a = 10; // OK
        // ref_b = 20; // Error: ref_b is const because captured by value in non-mutable lambda (unless lambda is mutable)
                     // If mutable, ref_b is a modifiable copy.
        ref_b = 200; // OK because lambda is mutable
        ref_c = 30; // OK
    };
    default_ref_capture();
    std::cout << "After default_ref_capture: ref_a=" << ref_a << ", ref_b=" << ref_b << ", ref_c=" << ref_c << std::endl;


    // 7. Mutable lambda: allows modification of by-value captures
    int mutable_val = 100;
    auto mutable_lambda = [mutable_val]() mutable {
        mutable_val += 10; // Modifies the lambda's copy of mutable_val
        std::cout << "Inside mutable_lambda, mutable_val = " << mutable_val << std::endl;
    };
    mutable_lambda();
    mutable_lambda(); // Call again to see the change persist within the lambda's state
    std::cout << "Outside mutable_lambda, original mutable_val = " << mutable_val << std::endl;

    // 8. Omitting trailing return type if it can be deduced (simple cases)
    auto simple_product = [](int a, int b) { return a * b; }; // return type 'int' is deduced
    std::cout << "Simple product: " << simple_product(4, 5) << std::endl;

    return 0;
}

/*
Explanation:
Lambda expressions, introduced in C++11, provide a concise way to define
anonymous functions (or function objects) directly where they are needed.
They are particularly useful for short functions that are passed as arguments
to algorithms or used for event handling, callbacks, etc.

Syntax: [capture_clause](parameters) -> return_type { body }
- capture_clause: Specifies which variables from the surrounding scope are
  accessible inside the lambda and how (by value or by reference).
  - []: No variables captured.
  - [=]: Capture all variables from the surrounding scope by value.
  - [&]: Capture all variables from the surrounding scope by reference.
  - [x, &y]: Capture 'x' by value and 'y' by reference.
  - [=, &y]: Capture 'y' by reference, and all others by value.
  - [&, x]: Capture 'x' by value, and all others by reference.
- parameters: The parameter list for the lambda, similar to a normal function.
- -> return_type: The return type of the lambda. This can be omitted if the
  compiler can deduce it (e.g., if the lambda body is just `return expression;`).
- { body }: The actual code of the lambda.

'mutable' keyword: Can be used after the parameter list to allow modification
of variables captured by value. By default, by-value captures are const within
the lambda.

How to compile:
g++ -std=c++11 lambda_expressions.cpp -o lambda_example
./lambda_example
*/
