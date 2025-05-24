# cpp20/CMakeLists.txt
message(STATUS "Processing cpp20 CMakeLists.txt")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
message(STATUS "  cpp20 CMAKE_CXX_STANDARD set to ${CMAKE_CXX_STANDARD}")

# List of C++20 example files (excluding module implementation for now)
set(CPP20_CORE_EXAMPLES_NO_MODULES
    core_language/concepts.cpp
    core_language/ranges.cpp
    core_language/coroutines.cpp
    core_language/three_way_comparison.cpp
    # core_language/modules_basic_usage.cpp # Handled separately
)

set(CPP20_LIB_EXAMPLES
    standard_library/std_format.cpp
    standard_library/std_span.cpp
    standard_library/std_latch.cpp
)

# Add executables for core language examples (excluding modules)
foreach(example_file ${CPP20_CORE_EXAMPLES_NO_MODULES})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(cpp20_${example_name} ${example_file})
    set_target_properties(cpp20_${example_name} PROPERTIES OUTPUT_NAME "${example_name}_cpp20")
    # message(STATUS "    Added C++20 core executable: ${example_name}_cpp20")

    if(example_name STREQUAL "coroutines")
        # Compiler flags for coroutines can vary.
        # GCC/Clang often use -fcoroutines. MSVC uses /await.
        # CMake doesn't have a standard way to enable coroutines yet.
        # We add flags based on compiler ID.
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
            target_compile_options(cpp20_${example_name} PRIVATE -fcoroutines)
            message(STATUS "    Adding -fcoroutines for ${example_name}_cpp20")
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            target_compile_options(cpp20_${example_name} PRIVATE /await)
            message(STATUS "    Adding /await for ${example_name}_cpp20")
        endif()
        # Coroutines example might use threads if awaitables schedule on them
        find_package(Threads REQUIRED)
        target_link_libraries(cpp20_${example_name} PRIVATE Threads::Threads)
        message(STATUS "    Linking Threads for ${example_name}_cpp20 (coroutines)")
    endif()
endforeach()

# Add executables for standard library examples
foreach(example_file ${CPP20_LIB_EXAMPLES})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(cpp20_lib_${example_name} ${example_file})
    set_target_properties(cpp20_lib_${example_name} PROPERTIES OUTPUT_NAME "${example_name}_cpp20_lib")
    # message(STATUS "    Added C++20 lib executable: ${example_name}_cpp20_lib")

    if(example_name STREQUAL "std_latch") # or other examples using std::thread
        find_package(Threads REQUIRED)
        target_link_libraries(cpp20_lib_${example_name} PRIVATE Threads::Threads)
        message(STATUS "    Linking Threads for ${example_name}_cpp20_lib")
    endif()
endforeach()


# --- C++20 Modules Handling ---
# CMake's support for C++20 modules is experimental and varies by version and compiler.
# This is a best-effort attempt for GCC and Clang.
# For MSVC, it's different and often involves /interface and /module:reference flags.

set(MATH_MODULE_SOURCE core_language/math_module.cppm)
set(MODULE_USAGE_SOURCE core_language/modules_basic_usage.cpp)
set(MODULE_TARGET_NAME cpp20_math_module) # Not used as a separate target in this simplified version
set(MODULE_USAGE_TARGET_NAME cpp20_modules_basic_usage)

# Check for basic module support (CMake 3.28+ has better native support)
# For older CMakes, this is more manual.
if(CMAKE_VERSION VERSION_LESS "3.28" AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
    message(WARNING "CMake version ${CMAKE_VERSION} has limited C++20 module support. Attempting manual configuration for GCC/Clang.")
    # This is a simplified approach. Real projects might need more robust handling.
    # For GCC, -fmodules-ts is often used. Clang also uses -fmodules-ts or specific flags.

    # 1. Create a target for the module interface
    #    This isn't a standard CMake way but simulates the need to compile the module first.
    #    A custom command or target might be used.
    #    With newer CMake, add_library(math_operations MODULE ${MATH_MODULE_SOURCE}) might work differently.

    # For this example, we'll try to compile them together, relying on compiler to handle order if possible,
    # or assume user compiles module interface manually first if using older CMake/compilers.
    # A more robust solution for older CMake would be to create a custom target for the .gcm/.pcm file.

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # For GCC, we often need to compile the module interface first to create a .gcm file.
        # This example assumes a simplified scenario where direct compilation might work for basic cases
        # or that advanced users will handle .gcm generation.
        # A proper setup would involve add_custom_command.
        message(STATUS "Module compilation with GCC requires -fmodules-ts. This example might need manual .gcm generation first if it fails directly.")
        add_executable(${MODULE_USAGE_TARGET_NAME} ${MODULE_USAGE_SOURCE} ${MATH_MODULE_SOURCE}) # Try compiling together
        set_target_properties(${MODULE_USAGE_TARGET_NAME} PROPERTIES OUTPUT_NAME "modules_basic_usage_cpp20")
        target_compile_options(${MODULE_USAGE_TARGET_NAME} PRIVATE -fmodules-ts)
        # target_link_libraries(${MODULE_USAGE_TARGET_NAME} PRIVATE ${MODULE_TARGET_NAME}) # If module was a library target

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        message(STATUS "Module compilation with Clang requires -fmodules-ts (or similar) and precompilation of .pcm. This example might need manual .pcm generation first if it fails directly.")
        add_executable(${MODULE_USAGE_TARGET_NAME} ${MODULE_USAGE_SOURCE} ${MATH_MODULE_SOURCE}) # Try compiling together
        set_target_properties(${MODULE_USAGE_TARGET_NAME} PROPERTIES OUTPUT_NAME "modules_basic_usage_cpp20")
        target_compile_options(${MODULE_USAGE_TARGET_NAME} PRIVATE -fmodules-ts -fprebuilt-module-path=${CMAKE_CURRENT_BINARY_DIR}) # Assuming .pcm might be output here or found
        # A proper setup would involve add_custom_command to generate PCM and then use it.
    else()
        message(WARNING "C++20 Modules example (${MODULE_USAGE_TARGET_NAME}) might not compile correctly with this CMake version and compiler (${CMAKE_CXX_COMPILER_ID}). Manual compilation or a newer CMake version might be required.")
    endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "Module compilation with MSVC requires /experimental:module or /std:c++latest and specific build ordering.")
    # MSVC needs /interface for module interface units and /module:reference for importers.
    # This is complex to set up generically without more advanced CMake features or specific project structure.
    # We will attempt a basic add_executable, but it will likely require manual project setup in VS or vcxproj adjustments.
    add_executable(${MODULE_USAGE_TARGET_NAME} ${MODULE_USAGE_SOURCE} ${MATH_MODULE_SOURCE})
    set_target_properties(${MODULE_USAGE_TARGET_NAME} PROPERTIES OUTPUT_NAME "modules_basic_usage_cpp20")
    # Attempt to add flags, but proper MSVC module compilation is more involved.
    target_compile_options(${MODULE_USAGE_TARGET_NAME} PRIVATE /std:c++latest /experimental:module) # Or just /std:c++20
    message(WARNING "MSVC module compilation for ${MODULE_USAGE_TARGET_NAME} is basic and may require manual IDE/project configuration.")

else # Newer CMake versions (>= 3.28) or other compilers
    # CMake 3.28+ has more built-in support for C++ named modules.
    # try_add_library with FILE_SET for module sources is the modern way.
    # For simplicity in this example for potentially older CMakes, we'll keep it basic.
    message(STATUS "Attempting basic module compilation for ${MODULE_USAGE_TARGET_NAME}. For robust module support, CMake 3.28+ is recommended.")
    add_executable(${MODULE_USAGE_TARGET_NAME} ${MODULE_USAGE_SOURCE} ${MATH_MODULE_SOURCE})
    set_target_properties(${MODULE_USAGE_TARGET_NAME} PROPERTIES OUTPUT_NAME "modules_basic_usage_cpp20")
    # No special flags added here, relying on compiler's default C++20 mode to handle modules if possible.
endif()


message(STATUS "Finished processing cpp20 CMakeLists.txt")
