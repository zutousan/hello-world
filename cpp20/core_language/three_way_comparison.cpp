// three_way_comparison.cpp
#include <iostream>
#include <compare>  // Key header for std::strong_ordering, std::weak_ordering, etc.
#include <string>
#include <vector>
#include <set>      // For std::set to demonstrate ordering
#include <algorithm> // For std::transform
#include <cctype>    // For ::tolower
#include <limits>    // For std::numeric_limits
#include <type_traits> // For std::is_same_v

// --- 1. Defaulted three-way comparison (operator<=>) ---
struct Point_Defaulted {
    int x;
    double y;

    auto operator<=>(const Point_Defaulted& other) const = default;
};

void test_point_defaulted(const Point_Defaulted& p1, const Point_Defaulted& p2, const std::string& p1_name, const std::string& p2_name) {
    // Corrected std::cout formatting and quoting
    std::cout << "Comparing " << p1_name << " {" << p1.x << ", " << p1.y << "} and "
              << p2_name << " {" << p2.x << ", " << p2.y << "}:" << std::endl;

    std::cout << "  " << p1_name << " == " << p2_name << ": " << std::boolalpha << (p1 == p2) << std::endl;
    std::cout << "  " << p1_name << " != " << p2_name << ": " << std::boolalpha << (p1 != p2) << std::endl;
    std::cout << "  " << p1_name << " <  " << p2_name << ": " << std::boolalpha << (p1 < p2) << std::endl;
    std::cout << "  " << p1_name << " <= " << p2_name << ": " << std::boolalpha << (p1 <= p2) << std::endl;
    std::cout << "  " << p1_name << " >  " << p2_name << ": " << std::boolalpha << (p1 > p2) << std::endl;
    std::cout << "  " << p1_name << " >= " << p2_name << ": " << std::boolalpha << (p1 >= p2) << std::endl;

    auto result = (p1 <=> p2);
    if (result == 0) std::cout << "  " << p1_name << " <=> " << p2_name << " is 'equivalent/equal'" << std::endl;
    else if (result < 0) std::cout << "  " << p1_name << " <=> " << p2_name << " is 'less'" << std::endl;
    else if (result > 0) std::cout << "  " << p1_name << " <=> " << p2_name << " is 'greater'" << std::endl;
    
    if constexpr (std::is_same_v<decltype(result), std::strong_ordering>) {
        std::cout << "  Comparison category: std::strong_ordering" << std::endl;
    } else if constexpr (std::is_same_v<decltype(result), std::weak_ordering>) {
        std::cout << "  Comparison category: std::weak_ordering" << std::endl;
    } else if constexpr (std::is_same_v<decltype(result), std::partial_ordering>) {
        std::cout << "  Comparison category: std::partial_ordering" << std::endl;
    }
}

// --- 2. Custom three-way comparison ---
struct Person_Custom {
    std::string name;
    int age;

    std::strong_ordering operator<=>(const Person_Custom& other) const {
        if (auto cmp = name <=> other.name; cmp != 0) {
            return cmp;
        }
        return age <=> other.age;
    }

    bool operator==(const Person_Custom& other) const {
        return (*this <=> other) == 0;
    }
};

void test_person_custom(const Person_Custom& p1, const Person_Custom& p2, const std::string& p1_name, const std::string& p2_name) {
    // Corrected std::cout formatting and quoting
    std::cout << "Comparing " << p1_name << "{\"" << p1.name << "\", " << p1.age << "} and "
              << p2_name << "{\"" << p2.name << "\", " << p2.age << "}:" << std::endl;
    std::cout << "  " << p1_name << " < " << p2_name << ": " << (p1 < p2) << std::endl;
    std::cout << "  " << p1_name << " == " << p2_name << ": " << (p1 == p2) << std::endl;
}

// --- 3. Different Comparison Categories ---
struct CaseInsensitiveString {
    std::string s;

    std::weak_ordering operator<=>(const CaseInsensitiveString& other) const {
        std::string s_lower = s;
        std::string other_s_lower = other.s;
        std::transform(s_lower.begin(), s_lower.end(), s_lower.begin(), ::tolower);
        std::transform(other_s_lower.begin(), other_s_lower.end(), other_s_lower.begin(), ::tolower);

        if (s_lower < other_s_lower) return std::weak_ordering::less;
        if (s_lower > other_s_lower) return std::weak_ordering::greater;
        return std::weak_ordering::equivalent;
    }
    bool operator==(const CaseInsensitiveString& other) const {
        return (*this <=> other) == 0;
    }
};

int main() {
    std::cout << "--- C++20 Three-Way Comparison (Spaceship Operator <=>) ---" << std::endl;

    std::cout << "\n1. Defaulted operator<=> for Point_Defaulted:" << std::endl;
    Point_Defaulted p1d{1, 2.0}, p2d{1, 3.0}, p3d{2, 1.0}, p4d{1, 2.0};
    test_point_defaulted(p1d, p2d, "p1d", "p2d");
    test_point_defaulted(p1d, p3d, "p1d", "p3d");
    test_point_defaulted(p1d, p4d, "p1d", "p4d");

    std::cout << "\n2. Custom operator<=> for Person_Custom:" << std::endl;
    Person_Custom pc1{"Alice", 30}, pc2{"Bob", 25}, pc3{"Alice", 28}, pc4{"Alice", 30};
    test_person_custom(pc1, pc2, "pc1", "pc2");
    test_person_custom(pc1, pc3, "pc1", "pc3");
    test_person_custom(pc1, pc4, "pc1", "pc4");

    std::set<Person_Custom> person_set;
    person_set.insert(pc1);
    person_set.insert(pc2);
    person_set.insert(pc3);
    std::cout << "Set of Persons (ordered by name, then age):" << std::endl;
    for (const auto& p : person_set) {
        // Corrected std::cout formatting and quoting
        std::cout << "  {\"" << p.name << "\", " << p.age << "}" << std::endl;
    }

    std::cout << "\n3. Weak ordering example:" << std::endl;
    CaseInsensitiveString cis1{"Hello"}, cis2{"hello"}, cis3{"World"};
    // Corrected std::cout formatting and quoting
    std::cout << "Comparing cis1(\"Hello\") and cis2(\"hello\"):" << std::endl;
    std::cout << "  cis1 <=> cis2 is " << ((cis1 <=> cis2) == 0 ? "equivalent" : "not equivalent") << std::endl;
    std::cout << "  cis1 == cis2: " << std::boolalpha << (cis1 == cis2) << std::endl;

    // Corrected std::cout formatting and quoting
    std::cout << "Comparing cis1(\"Hello\") and cis3(\"World\"):" << std::endl;
    std::cout << "  cis1 < cis3: " << std::boolalpha << (cis1 < cis3) << std::endl;

    float f1 = 1.0f, f_nan = std::numeric_limits<float>::quiet_NaN(); // Use float literal
    auto partial_res = f1 <=> f_nan;
    if (partial_res == std::partial_ordering::unordered) {
        std::cout << "Comparing " << f1 << " and NaN results in std::partial_ordering::unordered." << std::endl;
    }
    std::cout << "  f1 < NaN: " << (f1 < f_nan) << std::endl;
    std::cout << "  f1 == NaN: " << (f1 == f_nan) << std::endl;
    std::cout << "  f1 > NaN: " << (f1 > f_nan) << std::endl;

    return 0;
}

/*
Explanation:
The three-way comparison operator, `operator<=>` (spaceship operator), C++20.
Key Header: `<compare>` (for `std::strong_ordering`, etc.)
- Simplifies comparisons: define `operator<=>`, compiler can synthesize `==, !=, <, <=, >, >=`.
- Returns comparison category: `std::strong_ordering`, `std::weak_ordering`, `std::partial_ordering`.

Defaulted `operator<=>`:
- `auto operator<=>(const MyType&) const = default;`
- Compiler generates lexicographical comparison of members.
- `operator==` also implicitly defaulted. Other ops synthesized from `<=>`.

Custom `operator<=>`:
- Define your logic.
- Must also define `operator==` (others synthesized from custom `<=>` and your `==`).

Comparison Categories:
- `std::strong_ordering`: Equivalent values are substitutable (e.g., integers). Values: `less, equal, greater`.
- `std::weak_ordering`: Equivalent values not necessarily substitutable (e.g., case-insensitive strings). Values: `less, equivalent, greater`.
- `std::partial_ordering`: Not all values comparable (e.g., float NaN). Values: `less, equivalent, greater, unordered`.

How to compile:
g++ -std=c++20 three_way_comparison.cpp -o three_way_comparison_example
*/
