# CMakeLists.txt (root)
cmake_minimum_required(VERSION 3.16) # Min version for better C++20 support, can be adjusted

project(CppFeaturesShowcase LANGUAGES CXX)

# Set the default C++ standard for the whole project.
# Individual subdirectories can override this if they need an older standard
# for specific examples, though generally, we compile examples with their target standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS Off) # Avoid compiler-specific extensions like -std=gnu++20

# Optional: Enable testing with CTest
# enable_testing()

# Message to user
message(STATUS "Building C++ Features Showcase (C++11 to C++20)")
message(STATUS "Root CMAKE_CXX_STANDARD set to ${CMAKE_CXX_STANDARD}")

# Add subdirectories for each C++ version
add_subdirectory(cpp11)
add_subdirectory(cpp14)
add_subdirectory(cpp17)
add_subdirectory(cpp20)

message(STATUS "Finished processing root CMakeLists.txt")
