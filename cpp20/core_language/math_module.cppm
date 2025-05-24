// cpp20/core_language/math_module.cppm
export module math_operations; // Declares a module named math_operations

// Exported function
export int add(int a, int b) {
    return a + b;
}

// Non-exported (module-private) function
int subtract_internal(int a, int b) {
    return a - b;
}

// Exported function that uses an internal function
export int subtract_public(int a, int b) {
    return subtract_internal(a, b);
}

// Exported constant
export const double PI = 3.1415926535;

// Exported struct
export struct Point {
    int x;
    int y;
};
