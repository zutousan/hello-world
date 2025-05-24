// std_make_unique.cpp
#include <iostream>
#include <memory>   // For std::unique_ptr, std::make_unique
#include <string>
#include <vector>

// A sample class to use with std::make_unique
struct Widget {
    int id;
    std::string name;
    std::vector<double> data;

    Widget(int i, const std::string& n) : id(i), name(n) {
        std::cout << "Widget " << id << " ('" << name << "') constructed with id and name." << std::endl;
    }

    Widget(int i, std::string&& n, std::vector<double>&& d) : id(i), name(std::move(n)), data(std::move(d)) {
        std::cout << "Widget " << id << " ('" << name << "') constructed with id, rvalue name, and rvalue vector. Data size: " << data.size() << std::endl;
    }

    ~Widget() {
        std::cout << "Widget " << id << " ('" << name << "') destroyed." << std::endl;
    }

    void show() const {
        std::cout << "Widget ID: " << id << ", Name: '" << name << "'";
        if (!data.empty()) {
            std::cout << ", Data[0]: " << data[0]; // Show first element if data exists
        }
        std::cout << std::endl;
    }
};

// A custom deleter for unique_ptr (for demonstration, though not directly related to make_unique)
struct WidgetDeleter {
    void operator()(Widget* w) const {
        std::cout << "Custom WidgetDeleter called for Widget " << w->id << "." << std::endl;
        delete w;
    }
};

int main() {
    std::cout << "--- Using std::make_unique (C++14) ---" << std::endl;

    // 1. Basic usage: Creating a unique_ptr to a single object
    auto up1 = std::make_unique<Widget>(1, "ModernWidget");
    if (up1) {
        up1->show();
    }

    // 2. Arguments are perfectly forwarded to the constructor of Widget
    std::string temp_name = "TempName";
    std::vector<double> temp_data = {1.1, 2.2, 3.3};
    auto up2 = std::make_unique<Widget>(2, std::move(temp_name), std::move(temp_data));
    if (up2) {
        up2->show();
    }
    std::cout << "temp_name after move: \"" << temp_name << "\"" << std::endl; // Corrected quotes, Likely empty

    // 3. Creating a unique_ptr to a dynamically-allocated array (C++14 feature for make_unique)
    std::cout << "\n(std::make_unique<T[]> for default-initialized arrays example commented out as Widget has no default ctor)" << std::endl; // Corrected newline

    // For C++14, to initialize an array with non-default values using unique_ptr,
    // you might do something like this (though not using make_unique for the array itself):
    std::unique_ptr<Widget*[]> owner_of_pointers(new Widget*[2]);
    owner_of_pointers[0] = new Widget(3, "ArrayElem1_RawNew");
    owner_of_pointers[1] = new Widget(4, "ArrayElem2_RawNew");
    
    // Manually delete the Widgets since owner_of_pointers only owns the array of raw pointers
    delete owner_of_pointers[0];
    delete owner_of_pointers[1];
    // The unique_ptr `owner_of_pointers` will correctly delete[] the array of pointers it owns.

    std::cout << "\n--- Benefits of std::make_unique ---" << std::endl; // Corrected newline
    std::cout << "1. Exception Safety: `make_unique` helps prevent resource leaks in expressions involving multiple allocations." << std::endl;
    std::cout << "2. Conciseness: Less verbose than using `new` directly with `std::unique_ptr`." << std::endl;

    // 4. std::unique_ptr with custom deleters
    std::unique_ptr<Widget, WidgetDeleter> up_custom_deleter(new Widget(7, "CustomDelete"), WidgetDeleter());
    if (up_custom_deleter) {
        up_custom_deleter->show();
    }
    std::cout << "\n(std::make_unique does not directly support custom deleters; use unique_ptr constructor for that.)" << std::endl; // Corrected newline

    std::cout << "\nEnd of main. Widgets will be destroyed by unique_ptrs." << std::endl; // Corrected newline
    return 0;
}

/*
Explanation:
`std::make_unique` is a utility function template introduced in C++14 (defined
in the `<memory>` header). It's the recommended way to create instances of
`std::unique_ptr`.

Syntax:
```cpp
// For single objects
template<class T, class... Args>
std::unique_ptr<T> make_unique(Args&&... args);

// For arrays of unknown bound (T[]) - objects are default-initialized in C++14/17
// C++20 adds overloads for value-initialization etc.
template<class T>
std::unique_ptr<T> make_unique(size_t size); // T is U[]

// For arrays of known bound (T[N]) - objects are default-initialized
// This form is less common. std::array is often preferred for fixed-size arrays.
// Use std::make_unique<U[]>(N) instead for dynamic arrays.
```

Key Benefits:

1.  Exception Safety:
    -   `std::make_unique` provides better exception safety in certain complex
        expressions compared to manually using `new` with `std::unique_ptr`.
    -   Consider a function call: `foo(std::unique_ptr<A>(new A()), std::unique_ptr<B>(new B()))`.
        If `new B()` throws an exception after `new A()` succeeds, the memory
        for `A` could be leaked because its managing `unique_ptr` hasn't been
        constructed yet.
    -   `foo(std::make_unique<A>(), std::make_unique<B>())` avoids this.

2.  Conciseness and Readability:
    -   It's less verbose:
        `auto ptr = std::make_unique<MyType>(arg1, arg2);`
        vs.
        `std::unique_ptr<MyType> ptr(new MyType(arg1, arg2));`
    -   Avoids repeating the type `MyType`.

3.  No `new` or `delete` in User Code:
    -   Promotes a cleaner style where raw `new` and `delete` are minimized.

Usage:
-   Single Objects:
    `auto widget_ptr = std::make_unique<Widget>(arg1, arg2);`
    The arguments `arg1, arg2` are perfectly forwarded to the constructor of `Widget`.

-   Dynamically-Sized Arrays (`Type[]`):
    `auto arr_ptr = std::make_unique<int[]>(10);`
    In C++14, for object types, `std::make_unique<MyType[]>(N)` creates an array
    of `N` default-initialized objects. If `MyType` has no default constructor,
    this is a compile error.

Custom Deleters:
-   `std::make_unique` does *not* support creating `std::unique_ptr`s with custom
    deleters. For custom deleters, you must still use the `std::unique_ptr`
    constructor directly:
    `std::unique_ptr<MyType, MyDeleter> ptr(new MyType(), MyDeleter());`

Recommendation:
Always prefer `std::make_unique` over direct use of `new` when creating
`std::unique_ptr`s for single objects or default-initialized arrays, unless a
custom deleter is required.

How to compile:
g++ -std=c++14 std_make_unique.cpp -o std_make_unique_example
./std_make_unique_example
*/
