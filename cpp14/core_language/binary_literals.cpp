// binary_literals.cpp
#include <iostream>
#include <bitset> // For printing numbers in binary format

int main() {
    std::cout << "--- C++14 Binary Literals ---" << std::endl;

    // 1. Basic Binary Literals
    int val1 = 0b1010;
    int val2 = 0B11110000;
    unsigned int val3 = 0b11111111111111111111111111111111;

    std::cout << "0b1010 in decimal: " << val1 << " (binary: " << std::bitset<4>(val1) << ")" << std::endl;
    std::cout << "0B11110000 in decimal: " << val2 << " (binary: " << std::bitset<8>(val2) << ")" << std::endl;
    std::cout << "0b... (32 ones) in decimal: " << val3 << " (binary: " << std::bitset<32>(val3) << ")" << std::endl;

    // 2. Using Digit Separators (single quote ') - Also a C++14 feature
    long long val4 = 0b1010'0000'1111'0000;
    int val5 = 0B0001'1110;

    std::cout << "0b1010'0000'1111'0000 in decimal: " << val4 
              << " (binary: " << std::bitset<16>(val4) << ")" << std::endl;
    std::cout << "0B0001'1110 in decimal: " << val5 
              << " (binary: " << std::bitset<8>(val5) << ")" << std::endl;

    // 3. Type Suffixes
    unsigned int ui_val = 0b1100'0101U;
    long l_val = 0b1000000000000000L;
    unsigned long long ull_val = 0b10101010'11110000'01010101'10011001ULL;

    std::cout << "0b1100'0101U (unsigned int): " << ui_val 
              << " (binary: " << std::bitset<8>(ui_val) << ")" << std::endl;
    std::cout << "0b1000000000000000L (long): " << l_val 
              << " (binary: " << std::bitset<16>(l_val) << ")" << std::endl;
    std::cout << "0b...ULL (unsigned long long): " << ull_val 
              << " (binary: " << std::bitset<32>(ull_val) << ")" << std::endl;

    // 4. Use in bitwise operations
    const int FLAG_A = 0b0001;
    const int FLAG_B = 0b0010;
    const int FLAG_C = 0b0100;
    const int FLAG_D = 0b1000;

    int current_flags = FLAG_A | FLAG_C;
    // Corrected newline formatting in the following cout statement
    std::cout << "\n--- Bitwise Operations ---" << std::endl; // MANUALLY CORRECTED to ensure \n is part of the string
    std::cout << "current_flags (A|C): " << std::bitset<4>(current_flags) 
              << " (decimal: " << current_flags << ")" << std::endl;

    if (current_flags & FLAG_C) {
        std::cout << "FLAG_C is set." << std::endl;
    }
    if (!(current_flags & FLAG_B)) {
        std::cout << "FLAG_B is not set." << std::endl;
    }

    current_flags |= FLAG_D;
    std::cout << "current_flags after |= FLAG_D: " << std::bitset<4>(current_flags)
              << " (decimal: " << current_flags << ")" << std::endl;
    
    current_flags &= ~FLAG_A;
    std::cout << "current_flags after &= ~FLAG_A: " << std::bitset<4>(current_flags)
              << " (decimal: " << current_flags << ")" << std::endl;

    return 0;
}

/*
Explanation:
Binary literals, introduced in C++14, provide a way to represent integral
numbers directly in base-2 (binary) format. This is particularly useful when
working with bitmasks, hardware registers, or any situation where the binary
representation of a number is more intuitive than its decimal or hexadecimal form.

Syntax:
-   Binary literals start with a prefix `0b` or `0B`.
-   This is followed by a sequence of binary digits (0 or 1).
    Example: `int x = 0b101; // x is 5`

Digit Separators:
-   C++14 also introduced digit separators (the single quote `'`) for all numeric
    literals (decimal, hexadecimal, octal, and binary).
-   They can be used to group digits for improved readability, especially with
    long binary numbers.
-   The separator is ignored by the compiler.
    Example: `int y = 0b1101'0010'1001;`

Type Suffixes:
-   Like other integer literals, binary literals can have suffixes to specify
    their type (e.g., `U` for unsigned, `L` for long, `LL` for long long).
    Example: `unsigned int z = 0b101010U;`
             `long val = 0b10000000000000000000000000000000L;`

Use Cases:
-   Defining bitmasks or flags: `const int READ_PERMISSION = 0b0001;`
-   Interfacing with hardware where register values are specified in binary.
-   Educational purposes for understanding binary representations.
-   Any scenario where the bit pattern of a number is more important than its
    decimal value.

Before C++14, developers often had to use hexadecimal literals or calculate
decimal values, which could be less direct and more error-prone for binary patterns.

How to compile:
g++ -std=c++14 binary_literals.cpp -o binary_literals_example
./binary_literals_example
*/
