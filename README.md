# C++ Feature Examples: C++11 to C++20

This repository provides a collection of C++ code examples demonstrating key features introduced in C++11, C++14, C++17, and C++20. Each example is designed to be a small, self-contained program focusing on a specific feature.

## Project Structure

The examples are organized into directories based on the C++ standard in which the features were introduced:

-   `cpp11/`: Examples for C++11 features.
    -   `core_language/`: Features related to the core C++ language.
    -   `standard_library/`: Enhancements to the C++ Standard Library.
-   `cpp14/`: Examples for C++14 features.
    -   `core_language/`
    -   `standard_library/`
-   `cpp17/`: Examples for C++17 features.
    -   `core_language/`
    -   `standard_library/`
-   `cpp20/`: Examples for C++20 features.
    -   `core_language/`
    -   `standard_library/`

Each `.cpp` file includes an "Explanation" section in comments at the bottom, describing the feature and how to compile that specific example.

## Features Covered (Overview)

Below is a list of the primary features demonstrated in this collection. Please refer to the source files for detailed explanations and code.

### C++11

**Core Language:**
-   `auto` keyword (`auto.cpp`)
-   Lambda expressions (`lambda_expressions.cpp`)
-   Range-based for loops (`range_based_for_loop.cpp`)
-   `nullptr` keyword (`nullptr.cpp`)
-   Scoped enumerations (`enum class`) (`enum_class.cpp`)
-   `override` and `final` specifiers (`override_final.cpp`)
-   Rvalue references and move semantics (`rvalue_references_move_semantics.cpp`)
-   Initializer lists (`initializer_lists.cpp`)

**Standard Library:**
-   `std::thread` (`std_thread.cpp`)
-   `std::chrono` (durations, clocks, time points) (`std_chrono.cpp`)
-   Smart pointers (`std::unique_ptr`, `std::shared_ptr`, `std::weak_ptr`) (`smart_pointers.cpp`)
-   `std::tuple` (`std_tuple.cpp`)
-   `std::regex` (regular expressions) (`std_regex.cpp`)

### C++14

**Core Language:**
-   Generic lambdas (auto parameters) (`generic_lambdas.cpp`)
-   Binary literals (`0b...`) (`binary_literals.cpp`)
-   Function return type deduction (`auto func()`) (`return_type_deduction.cpp`)

**Standard Library:**
-   `std::make_unique` (`std_make_unique.cpp`)
-   `std::shared_timed_mutex` (`std_shared_timed_mutex.cpp`)

### C++17

**Core Language:**
-   Structured bindings (`structured_bindings.cpp`)
-   `if constexpr` (`if_constexpr.cpp`)
-   Inline variables (`inline_variables.cpp`)
-   Nested namespace definitions (`A::B::C`) (`nested_namespaces.cpp`)

**Standard Library:**
-   `std::filesystem` (`std_filesystem.cpp`)
-   `std::optional` (`std_optional.cpp`)
-   `std::variant` (`std_variant.cpp`)
-   `std::any` (`std_any.cpp`)
-   Parallel algorithms (execution policies) (`parallel_algorithms.cpp`)

### C++20

**Core Language:**
-   Concepts (`concepts.cpp`)
-   Ranges (`ranges.cpp`)
-   Coroutines (`co_await`, `co_yield`) (`coroutines.cpp`)
-   Three-way comparison (`operator<=>`) (`three_way_comparison.cpp`)
-   Modules (basic conceptual example) (`math_module.cppm`, `modules_basic_usage.cpp`)

**Standard Library:**
-   `std::format` (`std_format.cpp`)
-   `std::span` (`std_span.cpp`)
-   `std::latch` (and `std::barrier` mentioned) (`std_latch.cpp`)
-   *(Note: `std::osyncstream` is used in `std_latch.cpp`)*

## Compilation

Each example file contains specific compilation instructions in its comments, as compiler flags and support for features (especially newer ones like C++20 modules or coroutines) can vary.

**General Requirements:**
-   A C++ compiler that supports the respective C++ standard (C++11, C++14, C++17, C++20).
    -   **GCC (g++)**: Version 9+ recommended for C++20 features.
    -   **Clang (clang++)**: Version 10+ recommended for C++20 features.
    -   **MSVC (cl.exe)**: Visual Studio 2019 (16.10+) or 2022 recommended for C++20.
-   Standard Library support for the features.

**Typical Compilation Flags:**

-   **For C++11:** `g++ -std=c++11 filename.cpp -o output_name -pthread` (if using `std::thread`)
-   **For C++14:** `g++ -std=c++14 filename.cpp -o output_name -pthread`
-   **For C++17:** `g++ -std=c++17 filename.cpp -o output_name -pthread`
    -   For `<filesystem>` with GCC < 9: `g++ -std=c++17 filename.cpp -o output_name -lstdc++fs`
-   **For C++20:** `g++ -std=c++20 filename.cpp -o output_name -pthread`
    -   **Coroutines**: Often require an additional flag like `-fcoroutines` (GCC/Clang) or `/await` (MSVC).
    -   **Modules**: Require special compiler workflows. See comments in `modules_basic_usage.cpp`. Compilation is highly compiler-dependent. For example:
        ```bash
        # Conceptual GCC example for modules
        # g++ -std=c++20 -fmodules-ts -c cpp20/core_language/math_module.cppm -o math_operations.o 
        # (or precompile to a .gcm file first)
        # g++ -std=c++20 -fmodules-ts cpp20/core_language/modules_basic_usage.cpp math_operations.o -o modules_exe
        ```
        (Actual commands will vary. Refer to your compiler's documentation.)

**Building All Examples:**
A simple shell script could be used to compile all examples, but care must be taken due to differing C++ standards and specific flags. It's generally recommended to compile examples individually as per their embedded instructions.

## Building with CMake

CMake can be used to build all the examples in this project. This is generally recommended, especially for C++20 features like modules, as CMake can help manage compiler-specific flags and build order.

**Prerequisites:**
-   CMake (version 3.16 or higher recommended, 3.20+ for more robust C++20 modules support with some compilers like MSVC, 3.28+ for best native module support).
-   A C++ compiler that supports C++20 (GCC 9+, Clang 10+, MSVC VS2019 16.10+ or VS2022).

**General Build Steps:**

1.  **Create a build directory:**
    It's best practice to create a separate directory for the build files.
    ```bash
    mkdir build
    cd build
    ```

2.  **Run CMake to configure the project:**
    From within the `build` directory, run CMake, pointing it to the root of the project (where the main `CMakeLists.txt` is located).
    ```bash
    cmake ..
    ```
    -   You might need to specify a generator if you want to use a specific build system (e.g., `-G "Ninja"` or `-G "Visual Studio 17 2022"`).
    -   You can specify the C++ compiler if CMake doesn't pick up the correct one:
        `cmake .. -DCMAKE_CXX_COMPILER=g++-10`

3.  **Build the executables:**
    After CMake has configured the project, you can build all targets.
    ```bash
    cmake --build .
    ```
    Or, if using Makefiles:
    ```bash
    make
    ```
    Executables will be placed in the `build` directory, within subfolders corresponding to their C++ standard version (e.g., `build/cpp11/core_language/auto_cpp11`).

**Notes on Specific Features:**
-   **C++20 Modules:** The CMake configuration attempts a basic setup for the module example (`modules_basic_usage_cpp20`). Due to evolving CMake and compiler support, this particular target might require a very recent CMake version (3.28+) and a compatible compiler (GCC 11+/Clang 16+/MSVC VS2022 17.5+) for the most reliable build. If it fails, refer to the conceptual compilation notes in `cpp20/core_language/modules_basic_usage.cpp` and your compiler's documentation.
-   **Other C++20 Features (Coroutines, etc.):** The CMake files attempt to set necessary compiler flags (like `-fcoroutines` for GCC/Clang or `/await` for MSVC for coroutines).
-   **File System & Threads:** Linkage for `std::filesystem` and `std::thread` (pthreads) is handled by the CMake scripts.

The individual compilation instructions in each source file's comments are still valid for building examples one by one without CMake.

## Contributing

Feel free to suggest improvements, corrections, or new feature examples by opening an issue or pull request.

## License

This collection of examples is provided under the MIT License. See the (future) `LICENSE` file for details. (A LICENSE file will be added if this were a real public repo).
