# cpp17/CMakeLists.txt
message(STATUS "Processing cpp17 CMakeLists.txt")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
message(STATUS "  cpp17 CMAKE_CXX_STANDARD set to ${CMAKE_CXX_STANDARD}")

# List of C++17 example files
set(CPP17_CORE_EXAMPLES
    core_language/structured_bindings.cpp
    core_language/if_constexpr.cpp
    core_language/inline_variables.cpp
    core_language/nested_namespaces.cpp
)

set(CPP17_LIB_EXAMPLES
    standard_library/std_filesystem.cpp
    standard_library/std_optional.cpp
    standard_library/std_variant.cpp
    standard_library/std_any.cpp
    standard_library/parallel_algorithms.cpp
)

# Add executables for core language examples
foreach(example_file ${CPP17_CORE_EXAMPLES})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(cpp17_${example_name} ${example_file})
    set_target_properties(cpp17_${example_name} PROPERTIES OUTPUT_NAME "${example_name}_cpp17")
    # message(STATUS "    Added C++17 core executable: ${example_name}_cpp17")
endforeach()

# Add executables for standard library examples
foreach(example_file ${CPP17_LIB_EXAMPLES})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(cpp17_lib_${example_name} ${example_file})
    set_target_properties(cpp17_lib_${example_name} PROPERTIES OUTPUT_NAME "${example_name}_cpp17_lib")
    # message(STATUS "    Added C++17 lib executable: ${example_name}_cpp17_lib")

    # Specific linking for std_filesystem
    if(example_name STREQUAL "std_filesystem")
        # For GCC versions < 9, -lstdc++fs might be needed.
        # Modern CMake (3.14+) and compilers often handle this better.
        # We can try to link it conditionally or let modern CMake find it.
        # A robust way is to use find_library if needed, or rely on CXX_STANDARD.
        # For simplicity, let's try a common approach.
        if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
            target_link_libraries(cpp17_lib_${example_name} PRIVATE stdc++fs)
            message(STATUS "    Linking stdc++fs for ${example_name}_cpp17_lib (GCC < 9)")
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0) # Older Clangs might need -lc++fs
             target_link_libraries(cpp17_lib_${example_name} PRIVATE c++fs)
             message(STATUS "    Linking c++fs for ${example_name}_cpp17_lib (Clang < 7)")
        else()
            # For modern compilers, setting CMAKE_CXX_STANDARD to 17 should be enough.
            # If not, one might need: target_link_libraries(my_target PRIVATE Threads::Threads)
            # or find_package(Filesystem) if available and using imported targets.
            # For now, assume CMAKE_CXX_STANDARD is sufficient for most up-to-date compilers.
            message(STATUS "    std::filesystem for ${example_name}_cpp17_lib expected to link via CXX_STANDARD 17.")
        endif()
    endif()

    # Specific linking for parallel_algorithms
    if(example_name STREQUAL "parallel_algorithms")
        find_package(Threads REQUIRED)
        target_link_libraries(cpp17_lib_${example_name} PRIVATE Threads::Threads)
        message(STATUS "    Linking Threads for ${example_name}_cpp17_lib")
        # Some libstdc++ versions might require TBB for parallel algorithms.
        # Example: Check if TBB is found and link it.
        # find_package(TBB QUIET)
        # if(TBB_FOUND)
        // #    target_link_libraries(cpp17_lib_${example_name} PRIVATE TBB::tbb)
        // #    message(STATUS "    Linking TBB for ${example_name}_cpp17_lib")
        # else()
        // #    message(STATUS "    TBB not found, parallel algorithms might rely on another backend (e.g. pthreads, OpenMP) or be sequential.")
        # endif()
        # For simplicity, we'll rely on pthreads and the compiler's default parallel backend for now.
    endif()
endforeach()

message(STATUS "Finished processing cpp17 CMakeLists.txt")
