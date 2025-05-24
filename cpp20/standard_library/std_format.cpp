// std_format.cpp
#include <iostream>
#include <format>   // Key header for std::format (C++20)
#include <string>
#include <vector>
#include <chrono>   // For formatting time points (though direct chrono formatting is complex)

struct Point {
    int x, y;
};

// Provide a formatter specialization for Point
// This tells std::format how to format our custom type Point.
template <>
struct std::formatter<Point> {
    // parse() is called to parse format specifiers (e.g., if you had {0:spec})
    // For this simple Point, we don't have custom specifiers.
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin(); // Points to the end of the parsed range (empty here)
    }

    // format() is called to actually produce the formatted output
    template <typename FormatContext>
    auto format(const Point& p, FormatContext& ctx) const { // const is important
        return std::format_to(ctx.out(), "({x}, {y})", p.x, p.y);
        // Alternatively, build it piece by piece:
        // auto out = ctx.out();
        // *out++ = '(';
        // out = std::format_to(out, "{}", p.x);
        // *out++ = ',';
        // *out++ = ' ';
        // out = std::format_to(out, "{}", p.y);
        // *out++ = ')';
        // return out;
    }
};


int main() {
    std::cout << "--- std::format (C++20) ---" << std::endl;

    // 1. Basic formatting (similar to Python's f-strings or C#'s string interpolation)
    std::string name = "Alice";
    int age = 30;
    double height = 1.68;

    std::string s1 = std::format("User: {}, Age: {}, Height: {:.2f}m", name, age, height);
    std::cout << "s1: " << s1 << std::endl;

    // Positional arguments
    std::string s2 = std::format("User: {0}, Age: {1}. {0} is {1} years old.", name, age);
    std::cout << "s2 (positional): " << s2 << std::endl;

    // Named arguments (not directly in std::format, but can be emulated with maps or by passing args)
    // std::format does not have direct Python-like named arguments like "User: {name}".
    // Arguments are positional or indexed.


    // 2. Formatting various types
    std::cout << "\n2. Formatting various types:" << std::endl;
    std::cout << std::format("Integer: {}, Hex: {:#x}, Octal: {:#o}, Binary: {:#b}", 42, 42, 42, 42) << std::endl;
    std::cout << std::format("Floating point: {:.3f}, Scientific: {:e}, General: {:g}", 123.456789, 123.456789, 123.456789) << std::endl;
    std::cout << std::format("Boolean: {} or {}", true, false) << std::endl; // Default is 1/0
    std::cout << std::format("Boolean (alpha): {0:true}, {1:false}, {0:True}, {1:False}", true, false) << std::endl; // C++23 text output
    std::cout << std::format("Boolean (alpha C++20): {0}, {1}", true ? "true" : "false", false ? "true" : "false") << std::endl;


    // 3. Alignment, fill, width, precision
    std::cout << "\n3. Alignment, fill, width, precision:" << std::endl;
    std::cout << std::format("|{:<10}|", "left") << std::endl;    // Left align, width 10
    std::cout << std::format("|{:>10}|", "right") << std::endl;   // Right align, width 10
    std::cout << std::format("|{:^10}|", "center") << std::endl;  // Center align, width 10
    std::cout << std::format("|{:*^10}|", "fill") << std::endl;   // Center, width 10, fill with *
    std::cout << std::format("Value with sign: {:+}, No sign: {:}", 10, 10) << std::endl; // Sign
    std::cout << std::format("Precision for float: {:.2f}", 3.14159) << std::endl;


    // 4. Formatting pointers
    std::cout << "\n4. Formatting pointers:" << std::endl;
    int* ptr = &age;
    const void* void_ptr = ptr;
    std::cout << std::format("Pointer address: {}", static_cast<void*>(ptr)) << std::endl; // C-style cast for generic pointer
    std::cout << std::format("Pointer address (std::format default for void*): {}", void_ptr) << std::endl;


    // 5. Formatting a custom type (Point) using std::formatter specialization
    std::cout << "\n5. Formatting a custom type (Point):" << std::endl;
    Point p = {10, 20};
    std::string s_point = std::format("My point is: {}", p);
    std::cout << s_point << std::endl;
    std::cout << std::format("Another point: {}", Point{5, -5}) << std::endl;


    // 6. Outputting directly to an ostream (e.g., std::cout) using std::format_to
    std::cout << "\n6. Using std::format_to with std::cout:" << std::endl;
    std::cout << "Directly formatted output: ";
    std::format_to(std::ostreambuf_iterator<char>(std::cout), "Formatted int {} and string \"{}\"\n", 42, "example");


    // 7. Chrono integration (basic, more comprehensive in C++20 date/time formatting)
    // Direct formatting of std::chrono types like time_point is complex and relies on
    // how the chrono type itself overloads operator<< or if a specific formatter is provided.
    // C++20's <chrono> enhancements for formatting (std::chrono::format) are more powerful
    // for dates and times but are a separate sub-feature. std::format can call existing
    // stream insertion operators if available.
    std::cout << "\n7. Chrono (basic integration via existing stream ops if any):" << std::endl;
    auto now = std::chrono::system_clock::now();
    // std::cout << std::format("Current time: {}", now) << std::endl; // This may or may not compile/work well
                                                                  // depending on std::chrono::system_clock::time_point's
                                                                  // operator<< or formatter specialization.
                                                                  // Usually, you need more specific chrono formatting.
    std::cout << "(Direct std::format of std::chrono::time_point is complex; use dedicated chrono formatters)" << std::endl;


    // 8. Error handling (e.g., invalid format string)
    // std::format throws std::format_error on parsing errors.
    std::cout << "\n8. Error handling:" << std::endl;
    try {
        // Invalid format string (e.g., unmatched '{')
        std::string bad_format = std::format("Value: {", 10);
        std::cout << bad_format << std::endl;
    } catch (const std::format_error& e) {
        std::cout << "Caught std::format_error: " << e.what() << std::endl;
    }

    std::cout << "\nstd::format is a powerful and type-safe replacement for sprintf and iostreams for many formatting tasks." << std::endl;

    return 0;
}

/*
Explanation:
`std::format` (from the `<format>` header) is a C++20 feature that provides a
modern, type-safe, and extensible way to format strings. It aims to combine the
best aspects of C-style `printf`/`sprintf` (conciseness) and C++ iostreams
(type safety, extensibility) while avoiding their drawbacks.

Key Features:

1.  Python-like Format String Syntax:
    -   Uses curly braces `{}` as placeholders for arguments.
    -   `std::format("Hello, {}!", name);`
    -   Positional arguments: `std::format("Arg1: {1}, Arg0: {0}", arg0, arg1);`
    -   Format specifiers within braces control alignment, fill, width, precision, type, etc.
        `std::format("Value: {:#04x}", 42); // Hex, width 4, leading 0x, zero-padded`

2.  Type Safety:
    -   Arguments are checked at compile time (for basic types) or via the formatter
        specialization. Mismatches between format specifiers and argument types
        can often be caught at compile time or result in `std::format_error` at runtime.

3.  Extensibility for User-Defined Types:
    -   You can make your own types formattable by specializing `std::formatter<MyType>`.
    -   The specialization needs two methods:
        - `parse(std::format_parse_context& ctx)`: Parses format specifiers for `MyType`.
        - `format(const MyType& obj, FormatContext& ctx)`: Writes the formatted
          representation of `obj` to the output iterator `ctx.out()`.

4.  Performance:
    -   Designed to be efficient, often outperforming iostreams and `sprintf` in
        many common scenarios.

5.  Locale Independence by Default:
    -   Unlike iostreams, `std::format` is locale-independent by default, which
        is often desirable for internal logging or data serialization.
    -   Locale-specific formatting is possible if needed.

6.  Output Options:
    -   `std::format(...)`: Returns a `std::string`.
    -   `std::format_to(out_iterator, ...)`: Writes formatted output to any output
        iterator (e.g., `std::ostreambuf_iterator` for streams, `std::back_inserter`
        for containers).
    -   `std::formatted_size(...)`: Calculates the required buffer size.

Common Format Specifiers (inside `{}`):
-   `:[fill_char][align][sign][#][0][width][.precision][type]`
-   `align`: `<` (left), `>` (right), `^` (center)
-   `sign`: `+` (always show), `-` (only for negative, default), ` ` (space for positive)
-   `#`: Alternative form (e.g., `0x` for hex, `0` for octal)
-   `0`: Zero-padding when width is specified.
-   `width`: Minimum field width.
-   `.precision`: Precision for floating-point types or max width for strings.
-   `type`:
    - Integers: `b` (binary), `c` (char), `d` (decimal), `o` (octal), `x`/`X` (hex)
    - Floats: `f`/`F` (fixed), `e`/`E` (scientific), `g`/`G` (general), `a`/`A` (hex float)
    - String: `s` (default)
    - Pointer: `p`
    - Boolean (C++23 for text): `true`/`false` (e.g. `{0:true}`)

Error Handling:
-   Throws `std::format_error` if the format string is invalid or if an argument
    cannot be formatted according to the specifiers.

`std::format` is a significant improvement for string formatting in C++, offering
a safer, more powerful, and often more readable alternative to older methods.

How to compile:
g++ -std=c++20 std_format.cpp -o std_format_example
(or clang++ -std=c++20)
Ensure your compiler and standard library fully support C++20 <format>.
(Some earlier C++20 compilers might have had partial support or required specific flags).
*/
