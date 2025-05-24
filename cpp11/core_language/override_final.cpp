// override_final.cpp
#include <iostream>

// Base class
class Base {
public:
    virtual void print() const {
        std::cout << "Base::print()" << std::endl;
    }

    virtual void foo() {
        std::cout << "Base::foo()" << std::endl;
    }

    virtual void bar() const {
        std::cout << "Base::bar()" << std::endl;
    }

    // A function we intend to make non-overridable in a derived class later
    virtual void configure() {
        std::cout << "Base::configure()" << std::endl;
    }
};

// Derived class
class Derived : public Base {
public:
    // 1. 'override' specifier:
    // Ensures this function is actually overriding a virtual function from a base class.
    // If the signature doesn't match a base class virtual function, it's a compile-time error.
    void print() const override { // Correctly overrides Base::print()
        std::cout << "Derived::print() (override)" << std::endl;
    }

    // void print() override { // Compile Error: const qualifier mismatch, not an override of Base::print() const
    //     std::cout << "Derived::print() (non-const, trying to override)" << std::endl;
    // }

    // void fo0() override { // Compile Error: Typo in name, 'fo0' does not override any Base function.
    //    std::cout << "Derived::fo0() (typo, trying to override)" << std::endl;
    // }

    void foo() override { // Correctly overrides Base::foo()
        std::cout << "Derived::foo() (override)" << std::endl;
    }

    // If 'bar' in Base was not virtual, 'override' here would cause a compile error.
    // If 'bar' in Base was virtual but had a different signature, 'override' would also error.
    void bar() const override {
        std::cout << "Derived::bar() (override)" << std::endl;
    }
    
    // 2. 'final' specifier on a virtual function:
    // Prevents this function from being further overridden in any subsequent derived classes.
    void configure() final {
        std::cout << "Derived::configure() (final)" << std::endl;
    }
};

// Further derived class
class MoreDerived : public Derived {
public:
    void print() const override { // OK: Overrides Derived::print()
        std::cout << "MoreDerived::print()" << std::endl;
    }

    void foo() override { // OK: Overrides Derived::foo()
        std::cout << "MoreDerived::foo()" << std::endl;
    }

    // void configure() override { // Compile Error: Derived::configure() is marked 'final'
    //     std::cout << "MoreDerived::configure() (attempting to override final)" << std::endl;
    // }
};

// 3. 'final' specifier on a class:
// Prevents the class from being inherited from.
class SuperFinalClass final : public Base {
public:
    void print() const override {
        std::cout << "SuperFinalClass::print()" << std::endl;
    }
};

// class CantInherit : public SuperFinalClass { // Compile Error: SuperFinalClass is final
// public:
//     void doSomething() {
//         std::cout << "Trying to inherit from a final class." << std::endl;
//     }
// };


int main() {
    Base b;
    Derived d;
    MoreDerived md;
    SuperFinalClass sfc;

    std::cout << "--- Base Pointer Tests ---" << std::endl;
    Base* ptr_b = &b;
    Base* ptr_d = &d;
    Base* ptr_md = &md;
    Base* ptr_sfc = &sfc;

    ptr_b->print();
    ptr_d->print();   // Calls Derived::print()
    ptr_md->print();  // Calls MoreDerived::print()
    ptr_sfc->print(); // Calls SuperFinalClass::print()

    ptr_d->configure(); // Calls Derived::configure()

    std::cout << "\n--- Derived Pointer Tests ---" << std::endl;
    Derived* d_ptr = &d;
    d_ptr->configure(); // Calls Derived::configure() (which is final)

    // MoreDerived* md_ptr = &md;
    // md_ptr->configure(); // This would be an error if MoreDerived tried to override configure

    return 0;
}

/*
Explanation:
C++11 introduced two special identifiers, `override` and `final`, that provide
more control over virtual functions and class inheritance. They are not keywords
but are treated as special when used in specific contexts.

1.  `override` Specifier:
    -   When you declare a virtual function in a derived class that you intend
        to override a base class virtual function, you can append `override`
        to its declaration.
    -   Purpose: It acts as a compile-time check. If the function in the
        derived class does not have the exact same signature (name, parameter
        types, const/volatile qualifiers, ref-qualifiers) as a virtual function
        in a base class, the compiler will generate an error.
    -   Benefits:
        -   Prevents subtle bugs caused by typos or signature mismatches that
            would otherwise result in the function *not* overriding but rather
            *hiding* or becoming a new virtual function.
        -   Improves code readability and maintainability by clearly marking
            overriding functions.

2.  `final` Specifier:
    `final` can be used in two contexts:

    a.  On a Virtual Function:
        -   When `final` is appended to a virtual function declaration in a
            class, it prevents that function from being further overridden in
            any classes that derive from the current class.
        -   Example: `virtual void func() final;`
        -   Purpose: To ensure that a specific virtual function's implementation
            cannot be changed by subclasses.

    b.  On a Class Definition:
        -   When `final` is used after the class name in a class definition,
            it prevents the entire class from being used as a base class (i.e.,
            it cannot be inherited from).
        -   Example: `class MyClass final { /* ... */ };`
        -   Purpose: To prevent further derivation, often for design reasons or
            to ensure that the class's behavior cannot be altered through
            inheritance.

How to compile:
g++ -std=c++11 override_final.cpp -o override_final_example
./override_final_example
*/
