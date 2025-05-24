// rvalue_references_move_semantics.cpp
#include <iostream>
#include <vector>
#include <string>
#include <utility> // For std::move
#include <algorithm> // For std::min

// A simple class to demonstrate move semantics
class ResourceHolder {
public:
    int* data;
    size_t size;
    std::string name;

    // Constructor
    ResourceHolder(const std::string& n, size_t s) : name(n), size(s) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) data[i] = i;
        std::cout << "ResourceHolder '" << name << "' constructed. (size=" << size << ")" << std::endl;
    }

    // Destructor
    ~ResourceHolder() {
        if (data) {
            std::cout << "ResourceHolder '" << name << "' destructed. Releasing data." << std::endl;
            delete[] data;
            data = nullptr; // Good practice
            size = 0;
        } else {
            std::cout << "ResourceHolder '" << name << "' destructed. (already moved or empty)" << std::endl;
        }
    }

    // Copy Constructor (Deep Copy)
    ResourceHolder(const ResourceHolder& other) : name(other.name + "_copy"), size(other.size) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
        std::cout << "ResourceHolder '" << name << "' copy-constructed from '" << other.name << "'." << std::endl;
    }

    // Copy Assignment Operator (Deep Copy)
    ResourceHolder& operator=(const ResourceHolder& other) {
        std::cout << "ResourceHolder '" << name << "' copy-assigned from '" << other.name << "'." << std::endl;
        if (this == &other) {
            return *this; // Self-assignment check
        }
        delete[] data; // Release existing resource

        size = other.size;
        name = other.name + "_assigned_copy_to_" + name; // Modify name to show it's a copy
        data = new int[size];
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
        return *this;
    }

    // Move Constructor (using rvalue reference &&)
    // 'noexcept' is important for move operations to be used by STL containers in certain cases
    ResourceHolder(ResourceHolder&& other) noexcept : name(std::move(other.name)), size(other.size), data(other.data) {
        // Steal the resources from 'other'
        other.data = nullptr; // Leave 'other' in a valid but empty state
        other.size = 0;
        other.name = name + "_moved_from"; // Update other's name to reflect its state
        name += "_move_constructed";    // Update new object's name
        std::cout << "ResourceHolder '" << name << "' move-constructed from temporary/moved object." << std::endl;
    }

    // Move Assignment Operator (using rvalue reference &&)
    ResourceHolder& operator=(ResourceHolder&& other) noexcept {
        std::cout << "ResourceHolder '" << name << "' move-assigned from temporary/moved object." << std::endl;
        if (this == &other) {
            return *this; // Self-assignment check (though less common with rvalues)
        }
        delete[] data; // Release existing resource

        // Steal resources from 'other'
        data = other.data;
        size = other.size;
        name = std::move(other.name) + "_assigned_to_current"; // Move name

        // Leave 'other' in a valid but empty state
        other.data = nullptr;
        other.size = 0;
        other.name = name + "_moved_from_after_assign";

        return *this;
    }

    void print() const {
        if (data) {
            std::cout << "ResourceHolder '" << name << "' data: ";
            for (size_t i = 0; i < std::min(size, (size_t)5); ++i) std::cout << data[i] << " "; // Print first 5
            if (size > 5) std::cout << "...";
            std::cout << " (size=" << size << ")" << std::endl;
        } else {
            std::cout << "ResourceHolder '" << name << "' is empty." << std::endl;
        }
    }
};

ResourceHolder createResource(const std::string& name, size_t size) {
    return ResourceHolder(name, size); // Returns an rvalue
}

int main() {
    std::cout << "--- 1. Copy Semantics ---" << std::endl;
    ResourceHolder r1("R1", 3);
    ResourceHolder r2 = r1; // Calls Copy Constructor
    r2.name = "R2";
    ResourceHolder r3("R3_orig", 2);
    r3 = r1;              // Calls Copy Assignment Operator
    r3.name = "R3";

    r1.print();
    r2.print();
    r3.print();

    std::cout << "\n--- 2. Move Semantics with std::move ---" << std::endl;
    ResourceHolder r4("R4", 4);
    r4.print();
    ResourceHolder r5 = std::move(r4); // Calls Move Constructor. r4 is now in a "moved-from" state.
                                       // r4 should not be used unless reassigned.
    r5.name = "R5_moved_from_R4";
    r5.print();
    r4.print(); // r4 is now empty or in a valid but unspecified state

    ResourceHolder r6("R6_orig", 6);
    r6.print();
    ResourceHolder r7("R7_target", 1);
    r7.print();
    r7 = std::move(r6); // Calls Move Assignment. r6 is now in a "moved-from" state.
    r7.name = "R7_moved_from_R6";
    r7.print();
    r6.print();


    std::cout << "\n--- 3. Move Semantics with function returning by value (RVO/NRVO) ---" << std::endl;
    // Compilers often perform Return Value Optimization (RVO) or Named RVO (NRVO),
    // which can elide the move/copy constructor call.
    // However, the presence of a move constructor allows this optimization more broadly
    // and ensures efficiency if RVO doesn't occur.
    ResourceHolder r8 = createResource("R8_temp", 5); // Potentially calls Move Constructor (or RVO)
    r8.name = "R8";
    r8.print();


    std::cout << "\n--- 4. Using std::move with std::vector ---" << std::endl;
    std::vector<ResourceHolder> vec;
    // vec.push_back(ResourceHolder("V_Elem1", 2)); // Creates temporary, then move constructs into vector (if no RVO)
    // std::cout << "----" << std::endl;
    // ResourceHolder temp_v_elem("V_Elem2_source", 3);
    // vec.push_back(std::move(temp_v_elem)); // Move constructs into vector
    // std::cout << "Source element after move:" << std::endl;
    // temp_v_elem.print();
    // std::cout << "----" << std::endl;

    // More explicit demonstration for vector growth
    std::cout << "Creating v1, v2, v3 for vector push_back demo:" << std::endl;
    ResourceHolder v1("V1", 1), v2("V2", 2); //, v3("V3", 3);
    std::cout << "Vector capacity before push_back: " << vec.capacity() << std::endl;
    
    std::cout << "\nPushing v1 (lvalue) - will be copied:" << std::endl;
    vec.push_back(v1); // Calls copy constructor as v1 is an lvalue
    std::cout << "Vector capacity: " << vec.capacity() << std::endl;
    v1.print(); // v1 is unaffected
    
    std::cout << "\nPushing std::move(v2) (rvalue) - will be moved:" << std::endl;
    vec.push_back(std::move(v2)); // Calls move constructor as std::move(v2) is an rvalue
    std::cout << "Vector capacity: " << vec.capacity() << std::endl;
    v2.print(); // v2 is now in a moved-from state

    std::cout << "\nPushing temporary ResourceHolder - will be moved (or constructed in place):" << std::endl;
    vec.push_back(ResourceHolder("V_Temp", 3)); // Temporary is an rvalue, move constructor is used
    std::cout << "Vector capacity: " << vec.capacity() << std::endl;

    std::cout << "\nContents of vector:" << std::endl;
    for (const auto& item : vec) {
        item.print();
    }
    
    std::cout << "\n--- End of main ---" << std::endl;
    return 0;
}

/*
Explanation:
Rvalue References (&&):
-   C++11 introduced rvalue references (`Type&&`). An rvalue reference can bind
    to temporary objects (rvalues), such as literals, function return values by value,
    or objects explicitly cast to rvalues using `std::move`.
-   Lvalue references (`Type&`) bind to lvalues (objects with names/identifiable memory locations).

Move Semantics:
-   Allows resources (like dynamically allocated memory, file handles, sockets)
    to be "moved" from one object to another instead of being copied. This is
    much more efficient for expensive-to-copy resources.
-   It's typically implemented using:
    1.  Move Constructor: `ClassName(ClassName&& other)`
        -   Takes an rvalue reference to an object of the same class.
        -   "Steals" the resources from `other` (e.g., copies pointers/handles
            and sets the originals in `other` to a null/empty/valid state).
        -   Leaves `other` in a destructible and assignable state, but its
            specific contents are usually undefined after a move.
    2.  Move Assignment Operator: `ClassName& operator=(ClassName&& other)`
        -   Similar to the move constructor but for assignment.
        -   Releases its own resources before stealing from `other`.

`std::move`:
-   `std::move` is a utility function (in `<utility>`) that unconditionally
    casts its argument to an rvalue reference.
-   It doesn't actually *move* anything itself; it just signals that the object's
    resources *can be* moved from (i.e., it's an xvalue - an "expiring" value).
-   Use `std::move` when you want to indicate that an lvalue can be treated as
    a temporary, allowing a move operation to be invoked. After `std::move`,
    the original object should generally not be used again unless it's reassigned
    a new value, as its state is typically altered.

Benefits:
-   Performance: Drastically improves performance for operations involving
    temporary objects or when "transferring ownership" of resources (e.g.,
    returning large objects from functions, inserting into containers).
-   Enables efficient handling of resources for classes that manage them.

When are Move Operations Used?
-   When an object is initialized or assigned from an rvalue (e.g., a temporary
    object returned by a function).
-   When `std::move` is used to cast an lvalue to an rvalue.
-   Many Standard Library containers (like `std::vector`, `std::string`) and
    algorithms are optimized to use move semantics when appropriate, leading
    to significant performance gains (e.g., when a vector reallocates).

Rule of Three/Five/Zero:
-   Rule of Three (pre-C++11): If a class needs a destructor, a copy constructor,
    or a copy assignment operator, it probably needs all three.
-   Rule of Five (C++11): If a class defines any of the destructor, copy constructor,
    copy assignment operator, move constructor, or move assignment operator, it
    should probably define or `=delete` all five to handle copying and moving correctly.
-   Rule of Zero: Prefer classes that manage no resources directly (e.g., use RAII
    wrappers like smart pointers), so you don't need to write any of these special
    member functions yourself.

`noexcept`: It's good practice to mark move constructors and move assignment
operators as `noexcept` if they don't throw exceptions. This allows STL containers
to use them in more situations (e.g., `std::vector` can use move during reallocation
only if the move constructor is `noexcept`; otherwise, it might fall back to copying
for strong exception safety).

How to compile:
g++ -std=c++11 rvalue_references_move_semantics.cpp -o move_semantics_example
./move_semantics_example
*/
