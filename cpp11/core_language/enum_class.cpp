// enum_class.cpp
#include <iostream>

// Traditional C-style enum (unscoped)
enum ColorOld {
    RED,    // RED is in the global scope (or enclosing scope)
    GREEN,
    BLUE
};

// C++11 enum class (scoped enumeration)
enum class ColorNew {
    RED,    // ColorNew::RED
    GREEN,
    BLUE
};

// Another enum class, demonstrating no name collision
enum class TrafficLight {
    RED,    // TrafficLight::RED
    YELLOW,
    GREEN
};

// Enum class with specified underlying type
enum class AlertLevel : char {
    LOW = 'L',
    MEDIUM = 'M',
    HIGH = 'H'
};

int main() {
    // Using traditional enum
    ColorOld oldColor = RED; // RED is directly accessible
    if (oldColor == GREEN) {
        std::cout << "Old color is GREEN" << std::endl;
    } else {
        std::cout << "Old color is not GREEN (it's RED): " << oldColor << std::endl;
        // Implicit conversion to int:
        std::cout << "Old color as integer: " << oldColor << std::endl; // Outputs 0 for RED
    }
    // int x = RED; // Allowed: RED from ColorOld implicitly converts to int.

    // Using enum class
    ColorNew newColor = ColorNew::BLUE; // Must use scope resolution operator
    // if (newColor == ColorOld::BLUE) // Error: Cannot compare ColorNew and ColorOld directly
    // if (newColor == TrafficLight::RED) // Error: Different enum class types

    if (newColor == ColorNew::BLUE) {
        std::cout << "New color is ColorNew::BLUE" << std::endl;
    }

    // No implicit conversion to int for enum class
    // int y = ColorNew::GREEN; // Error: No implicit conversion
    // To get the underlying value, an explicit cast is needed:
    std::cout << "New color (ColorNew::BLUE) underlying value: "
              << static_cast<int>(newColor) << std::endl;

    TrafficLight light = TrafficLight::RED;
    std::cout << "Traffic light is: ";
    switch (light) {
        // Note: C++20 allows 'using enum TrafficLight;' inside the switch
        case TrafficLight::RED:
            std::cout << "RED";
            break;
        case TrafficLight::YELLOW:
            std::cout << "YELLOW";
            break;
        case TrafficLight::GREEN:
            std::cout << "GREEN";
            break;
    }
    std::cout << std::endl;
    // std::cout << "Traffic light RED underlying value: " << static_cast<int>(TrafficLight::RED) << std::endl;


    // Enum class with specified underlying type
    AlertLevel level = AlertLevel::HIGH;
    std::cout << "Alert level: " << static_cast<char>(level) << std::endl;
    std::cout << "Alert level (underlying value as int): " << static_cast<int>(static_cast<char>(level)) << std::endl;


    // Forward declaration of an enum class (possible because its size is known or can be specified)
    enum class ForwardDeclaredEnum : int; // Underlying type specified
    // enum class ForwardDeclaredEnum; // Also possible in C++11, underlying type defaults to int

    // ... later definition ...
    enum class ForwardDeclaredEnum : int { Val1, Val2 };
    ForwardDeclaredEnum fd_val = ForwardDeclaredEnum::Val1;
    std::cout << "Forward declared enum value: " << static_cast<int>(fd_val) << std::endl;


    return 0;
}

// Definition for a previously forward-declared enum (if not defined in main)
// enum class ForwardDeclaredEnum : int { Val1, Val2 };


/*
Explanation:
C++11 introduced "enum class" (also known as scoped enumerations) to address
several issues with traditional C-style enums:

1.  Scope:
    -   Traditional enum: Enumerators (e.g., RED, GREEN) are injected into the
        same scope as the enum itself. This can lead to name collisions if two
        enums in the same scope define enumerators with the same name.
        Example: `enum E1 { A }; enum E2 { A }; // Error`
    -   `enum class`: Enumerators are scoped within the enum. They must be
        accessed using the enum's name (e.g., `ColorNew::RED`). This prevents
        name clashes.
        Example: `enum class C1 { A }; enum class C2 { A }; // OK`

2.  Implicit Conversions:
    -   Traditional enum: Enumerators can implicitly convert to integer types.
        This can sometimes lead to logical errors if not intended.
        Example: `int i = RED; // Allowed`
    -   `enum class`: Enumerators do not implicitly convert to integers (or
        other types). An explicit `static_cast` is required if you need the
        underlying integer value. This enhances type safety.
        Example: `int i = static_cast<int>(ColorNew::RED); // Required`

3.  Underlying Type:
    -   Traditional enum: The underlying integral type is implementation-defined
        and chosen by the compiler (usually `int` or an unsigned type that can
        hold all enumerator values).
    -   `enum class`: You can explicitly specify the underlying integral type.
        If not specified, it defaults to `int`.
        Example: `enum class AlertLevel : char { LOW = 'L' };`

4.  Forward Declaration:
    -   Traditional enum: Cannot be forward-declared directly in a portable way
        before C++11 without specifying its underlying type (which wasn't standard).
    -   `enum class`: Can be forward-declared. If the underlying type is not
        specified in the forward declaration, it's assumed to be `int`.
        Example: `enum class MyEnum;` or `enum class MyEnum : unsigned int;`

Benefits of `enum class`:
-   Improved type safety.
-   Reduced risk of name collisions.
-   Clearer code by requiring explicit scoping.
-   Control over the underlying type.

How to compile:
g++ -std=c++11 enum_class.cpp -o enum_class_example
./enum_class_example
*/
