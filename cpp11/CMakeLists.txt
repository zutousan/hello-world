# cpp11/CMakeLists.txt
message(STATUS "Processing cpp11 CMakeLists.txt")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
message(STATUS "  cpp11 CMAKE_CXX_STANDARD set to ${CMAKE_CXX_STANDARD}")

# List of C++11 example files
set(CPP11_CORE_EXAMPLES
    core_language/auto.cpp
    core_language/lambda_expressions.cpp
    core_language/range_based_for_loop.cpp
    core_language/nullptr.cpp
    core_language/enum_class.cpp
    core_language/override_final.cpp
    core_language/rvalue_references_move_semantics.cpp
    core_language/initializer_lists.cpp
)

set(CPP11_LIB_EXAMPLES
    standard_library/std_thread.cpp
    standard_library/std_chrono.cpp
    standard_library/smart_pointers.cpp
    standard_library/std_tuple.cpp
    standard_library/std_regex.cpp
)

# Add executables for core language examples
foreach(example_file ${CPP11_CORE_EXAMPLES})
    get_filename_component(example_name ${example_file} NAME_WE) # NAME_WE gets name without extension
    add_executable(cpp11_${example_name} ${example_file})
    set_target_properties(cpp11_${example_name} PROPERTIES OUTPUT_NAME "${example_name}_cpp11")
    # message(STATUS "    Added C++11 core executable: ${example_name}_cpp11")
endforeach()

# Add executables for standard library examples
foreach(example_file ${CPP11_LIB_EXAMPLES})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(cpp11_lib_${example_name} ${example_file})
    set_target_properties(cpp11_lib_${example_name} PROPERTIES OUTPUT_NAME "${example_name}_cpp11_lib")
    # message(STATUS "    Added C++11 lib executable: ${example_name}_cpp11_lib")

    # Specific linking for std_thread
    if(example_name STREQUAL "std_thread")
        # FindThreads is a standard CMake module
        find_package(Threads REQUIRED)
        target_link_libraries(cpp11_lib_${example_name} PRIVATE Threads::Threads)
        message(STATUS "    Linking Threads for ${example_name}_cpp11_lib")
    endif()
endforeach()

message(STATUS "Finished processing cpp11 CMakeLists.txt")
