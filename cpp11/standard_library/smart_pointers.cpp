// smart_pointers.cpp
#include <iostream>
#include <memory>   // Main header for smart pointers (unique_ptr, shared_ptr, weak_ptr)
#include <vector>
#include <string>

// A sample class to use with smart pointers
class MyResource {
public:
    int id;
    std::string name;

    MyResource(int i, const std::string& n) : id(i), name(n) {
        std::cout << "MyResource " << id << " ('" << name << "') created." << std::endl;
    }

    ~MyResource() {
        std::cout << "MyResource " << id << " ('" << name << "') destroyed." << std::endl;
    }

    void show() const {
        std::cout << "MyResource " << id << " ('" << name << "') is here." << std::endl;
    }
};

// Function that takes unique_ptr by value (transfers ownership)
void process_unique(std::unique_ptr<MyResource> u_ptr) {
    if (u_ptr) {
        std::cout << "process_unique owns: ";
        u_ptr->show();
        // u_ptr goes out of scope here, and MyResource is destroyed
    } else {
        std::cout << "process_unique received a null unique_ptr." << std::endl;
    }
}

// Function that takes shared_ptr by value (shares ownership)
void process_shared(std::shared_ptr<MyResource> s_ptr) {
    if (s_ptr) {
        std::cout << "process_shared shares: ";
        s_ptr->show();
        std::cout << "  (use count in process_shared: " << s_ptr.use_count() << ")" << std::endl;
        // s_ptr goes out of scope here, use count decreases.
        // MyResource is destroyed only if this was the last shared_ptr.
    } else {
        std::cout << "process_shared received a null shared_ptr." << std::endl;
    }
}

// Function that returns a unique_ptr (transfers ownership out)
std::unique_ptr<MyResource> create_unique(int id, const std::string& name) {
    // return std::unique_ptr<MyResource>(new MyResource(id, name)); // Pre-C++14
    return std::make_unique<MyResource>(id, name); // C++14 style, but often implemented in C++11 compilers
                                                   // For strict C++11, use the line above or implement make_unique.
                                                   // Let's stick to new for pure C++11 core.
    // return std::unique_ptr<MyResource>(new MyResource(id, name));
}
// Custom make_unique for C++11 (simplified version)
template<typename T, typename... Args>
std::unique_ptr<T> make_unique_cpp11(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


int main() {
    std::cout << "--- std::unique_ptr ---" << std::endl;
    // `std::unique_ptr` provides exclusive ownership of a dynamically allocated object.
    // When the unique_ptr goes out of scope, it automatically deletes the managed object.
    // It's lightweight and has similar performance to a raw pointer.

    // 1. Creation
    // std::unique_ptr<MyResource> u1(new MyResource(1, "Unique One")); // Direct initialization
    std::unique_ptr<MyResource> u1 = make_unique_cpp11<MyResource>(1, "Unique One");


    if (u1) { // Check if it owns an object
        u1->show();
    }

    // 2. Ownership Transfer
    // unique_ptr cannot be copied, only moved.
    // std::unique_ptr<MyResource> u2 = u1; // Compile Error: Copy constructor is deleted.
    std::unique_ptr<MyResource> u2 = std::move(u1); // Transfers ownership from u1 to u2.
                                                    // u1 is now nullptr.

    std::cout << "After move: " << std::endl;
    if (!u1) {
        std::cout << "u1 is now null." << std::endl;
    }
    if (u2) {
        std::cout << "u2 now owns: ";
        u2->show();
    }

    // 3. Passing to function (transfers ownership)
    std::cout << "Processing u2..." << std::endl;
    process_unique(std::move(u2)); // u2 is moved into the function.
    // After this call, u2 in main is nullptr, and the resource is deleted by process_unique.
    if (!u2) {
        std::cout << "u2 is null after process_unique." << std::endl;
    }

    // 4. Returning from function
    // The create_unique function is defined but not used as is.
    // Instead, make_unique_cpp11 is used directly.
    std::unique_ptr<MyResource> u3 = make_unique_cpp11<MyResource>(3, "Unique Three from func");
    if (u3) {
        u3->show();
    }

    // 5. unique_ptr for arrays
    // std::unique_ptr<MyResource[]> u_arr(new MyResource[2]{ {4, "Arr1"}, {5, "Arr2"} }); // Won't work directly like this
    // Need to construct elements separately or use default constructor if available.
    // For array, use std::unique_ptr<T[]>
    std::unique_ptr<MyResource[]> u_arr = make_unique_cpp11<MyResource[]>(2); 
    // The above make_unique_cpp11 is not for arrays.
    // For arrays with make_unique (C++14) it would be: std::make_unique<MyResource[]>(2);
    // For C++11:
    std::unique_ptr<MyResource[]> u_arr_cpp11(new MyResource[2]{{4, "ArrElem1"}, {0, ""}}); // Second element default constructed then assigned
    u_arr_cpp11[0] = MyResource(4, "Arr C++11 One"); // Re-construct or assign if needed
    u_arr_cpp11[1] = MyResource(5, "Arr C++11 Two");
    
    if (u_arr_cpp11) {
        u_arr_cpp11[0].show();
        u_arr_cpp11[1].show();
    }
    // Array elements are automatically destroyed when u_arr_cpp11 goes out of scope.


    std::cout << "\n--- std::shared_ptr ---" << std::endl;
    // `std::shared_ptr` allows multiple pointers to share ownership of the same resource.
    // It keeps a reference count. The resource is deleted when the last shared_ptr owning it is destroyed or reset.

    // 1. Creation
    // Use std::make_shared (preferred for exception safety and performance)
    std::shared_ptr<MyResource> s1 = std::make_shared<MyResource>(10, "Shared Ten");
    std::cout << "s1 use count: " << s1.use_count() << std::endl; // Should be 1

    if (s1) {
        s1->show();
    }

    // 2. Copying (increases use count)
    std::shared_ptr<MyResource> s2 = s1; // s2 now also owns the resource.
    std::cout << "s1 use count after s2 = s1: " << s1.use_count() << std::endl;   // Should be 2
    std::cout << "s2 use count: " << s2.use_count() << std::endl;               // Should be 2

    s2->name = "Shared Ten (modified by s2)";
    s1->show(); // Shows modification as they point to the same object

    // 3. Passing to function (shares ownership)
    process_shared(s1);
    std::cout << "s1 use count after process_shared: " << s1.use_count() << std::endl; // Back to 2 (or 1 if s2 went out of scope)

    // 4. Resetting a shared_ptr
    std::cout << "Resetting s2..." << std::endl;
    s2.reset(); // s2 no longer owns the resource. Use count decreases.
    if (!s2) {
        std::cout << "s2 is now null." << std::endl;
    }
    std::cout << "s1 use count after s2.reset(): " << s1.use_count() << std::endl; // Should be 1

    // s1 goes out of scope, resource deleted as use count becomes 0.

    // Example with a vector of shared_ptr
    std::vector<std::shared_ptr<MyResource>> shared_vec;
    shared_vec.push_back(std::make_shared<MyResource>(20, "VecShared Twenty"));
    shared_vec.push_back(s1); // s1 is still valid and pointing to "Shared Ten"
    shared_vec.push_back(std::make_shared<MyResource>(21, "VecShared TwentyOne"));
    
    std::cout << "s1 use count after being added to vector: " << s1.use_count() << std::endl; // Should be 2

    std::cout << "Contents of shared_vec:" << std::endl;
    for (const auto& ptr : shared_vec) {
        if(ptr) ptr->show();
        std::cout << "  (use count: " << ptr.use_count() << ")" << std::endl;
    }

    std::cout << "\n--- std::weak_ptr (brief mention) ---" << std::endl;
    // `std::weak_ptr` is a non-owning ("weak") reference to an object managed by `std::shared_ptr`.
    // It's used to break circular dependencies between shared_ptrs.
    // To access the object, a weak_ptr must be converted to a shared_ptr using `lock()`.
    // This is a more advanced topic, typically covered after understanding shared_ptr.

    std::shared_ptr<MyResource> sp_owner = std::make_shared<MyResource>(30, "Weak Test Owner");
    std::weak_ptr<MyResource> wp_observer = sp_owner; // wp_observer points to the resource but doesn't own it

    std::cout << "sp_owner use count: " << sp_owner.use_count() << std::endl; // 1
    // std::cout << "wp_observer use count: " << wp_observer.use_count() << std::endl; // weak_ptr itself doesn't have use_count like shared_ptr, but refers to shared_ptr's count block

    if (auto temp_sp = wp_observer.lock()) { // Attempt to get a shared_ptr from weak_ptr
        std::cout << "Successfully locked weak_ptr: ";
        temp_sp->show();
        std::cout << "  (use count via locked ptr: " << temp_sp.use_count() << ")" << std::endl; // Becomes 2 temporarily
    } else {
        std::cout << "Failed to lock weak_ptr (resource might be gone)." << std::endl;
    }
    std::cout << "sp_owner use count after lock attempt: " << sp_owner.use_count() << std::endl; // Back to 1

    sp_owner.reset(); // Release ownership from sp_owner
    std::cout << "sp_owner reset." << std::endl;

    if (auto temp_sp = wp_observer.lock()) {
        std::cout << "Should not happen: Locked weak_ptr after owner reset." << std::endl;
    } else {
        std::cout << "Correctly failed to lock weak_ptr: resource is gone." << std::endl;
    }
    if (wp_observer.expired()){
        std::cout << "wp_observer has expired." << std::endl;
    }


    std::cout << "\nEnd of main. Resources will be automatically cleaned up by smart pointers." << std::endl;
    return 0;
}

/*
Explanation:
C++11 introduced smart pointers in the `<memory>` header to help manage dynamic
memory automatically and prevent common issues like memory leaks and dangling pointers.

1.  `std::unique_ptr<T>`:
    -   Represents exclusive ownership of a dynamically allocated object.
    -   When a `unique_ptr` goes out of scope, the object it points to is
        automatically deleted.
    -   Cannot be copied (copy constructor and copy assignment are deleted).
    -   Can be moved (move constructor and move assignment are available) using
        `std::move()`. This transfers ownership to another `unique_ptr`.
    -   Lightweight: Typically has the same size and performance characteristics
        as a raw pointer.
    -   Use Cases: Managing resources that should have a single owner, such as
        PIMPL idiom, factory functions returning unique objects.
    -   `std::make_unique<T>(args...)` (introduced in C++14, but often available
        or easily implementable in C++11) is the recommended way to create
        `unique_ptr`s for exception safety and conciseness. If not available,
        use `std::unique_ptr<T>(new T(args...))`.
    -   `unique_ptr<T[]>` specialization exists for managing dynamic arrays.

2.  `std::shared_ptr<T>`:
    -   Represents shared ownership of a dynamically allocated object.
    -   Multiple `shared_ptr` instances can point to the same object.
    -   Maintains a reference count (how many `shared_ptr`s point to the object).
    -   The object is deleted when the last `shared_ptr` pointing to it is
        destroyed or reset.
    -   Can be freely copied and assigned. Copying increments the reference count.
    -   `std::make_shared<T>(args...)` is the recommended way to create
        `shared_ptr`s. It's more efficient (allocates memory for the object and
        the control block in one go) and safer in some exception scenarios
        compared to `std::shared_ptr<T>(new T(args...))`.
    -   Use Cases: When multiple parts of a program need to share and manage the
        lifetime of an object (e.g., graph nodes, cached resources).

3.  `std::weak_ptr<T>`:
    -   A non-owning smart pointer that holds a "weak" reference to an object
        managed by `std::shared_ptr`.
    -   It does not affect the reference count of the `shared_ptr`.
    -   Used to break circular dependencies that can occur when two objects
        hold `shared_ptr`s to each other.
    -   To access the managed object, a `weak_ptr` must be converted to a
        `shared_ptr` by calling its `lock()` method.
    -   `expired()` method checks if the managed object has already been deleted.

General Benefits of Smart Pointers:
-   Automatic Memory Management: Reduces the risk of memory leaks (forgetting to
    `delete`) and dangling pointers (using a pointer after `delete`).
-   RAII (Resource Acquisition Is Initialization): Smart pointers embody RAII,
    ensuring resources are released when the smart pointer object goes out of scope.
-   Clear Ownership Semantics: `unique_ptr` for exclusive ownership, `shared_ptr`
    for shared ownership. This makes code easier to reason about.

Prefer smart pointers over raw pointers for owning dynamically allocated memory.

How to compile:
g++ -std=c++11 smart_pointers.cpp -o smart_pointers_example
./smart_pointers_example
*/
