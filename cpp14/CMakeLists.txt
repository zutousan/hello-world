# cpp14/CMakeLists.txt
message(STATUS "Processing cpp14 CMakeLists.txt")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
message(STATUS "  cpp14 CMAKE_CXX_STANDARD set to ${CMAKE_CXX_STANDARD}")

# List of C++14 example files
set(CPP14_CORE_EXAMPLES
    core_language/generic_lambdas.cpp
    core_language/binary_literals.cpp
    core_language/return_type_deduction.cpp
)

set(CPP14_LIB_EXAMPLES
    standard_library/std_make_unique.cpp
    standard_library/std_shared_timed_mutex.cpp
)

# Add executables for core language examples
foreach(example_file ${CPP14_CORE_EXAMPLES})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(cpp14_${example_name} ${example_file})
    set_target_properties(cpp14_${example_name} PROPERTIES OUTPUT_NAME "${example_name}_cpp14")
    # message(STATUS "    Added C++14 core executable: ${example_name}_cpp14")
endforeach()

# Add executables for standard library examples
foreach(example_file ${CPP14_LIB_EXAMPLES})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(cpp14_lib_${example_name} ${example_file})
    set_target_properties(cpp14_lib_${example_name} PROPERTIES OUTPUT_NAME "${example_name}_cpp14_lib")
    # message(STATUS "    Added C++14 lib executable: ${example_name}_cpp14_lib")

    # Specific linking for std_shared_timed_mutex
    if(example_name STREQUAL "std_shared_timed_mutex")
        find_package(Threads REQUIRED)
        target_link_libraries(cpp14_lib_${example_name} PRIVATE Threads::Threads)
        message(STATUS "    Linking Threads for ${example_name}_cpp14_lib")
    endif()
endforeach()

message(STATUS "Finished processing cpp14 CMakeLists.txt")
