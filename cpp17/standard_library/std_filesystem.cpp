// std_filesystem.cpp
#include <iostream>
#include <filesystem> // Key header for filesystem operations
#include <fstream>    // For std::ofstream to create a file
#include <vector>
#include <algorithm> // For std::sort

// Helper to print path information
void print_path_info(const std::filesystem::path& p) {
    std::cout << "Path: " << p << std::endl;
    // In C++20, p.string() can throw if not representable. p.u8string() is safer.
    // For C++17, p.string() is common.
    std::cout << "  String: " << p.string() << std::endl;
    std::cout << "  Root name: " << p.root_name().string() << std::endl;
    std::cout << "  Root directory: " << p.root_directory().string() << std::endl;
    std::cout << "  Root path: " << p.root_path().string() << std::endl;
    std::cout << "  Relative path: " << p.relative_path().string() << std::endl;
    std::cout << "  Parent path: " << p.parent_path().string() << std::endl;
    std::cout << "  Filename: " << p.filename().string() << std::endl;
    std::cout << "  Stem: " << p.stem().string() << std::endl;
    std::cout << "  Extension: " << p.extension().string() << std::endl;
    std::cout << "  Is absolute: " << std::boolalpha << p.is_absolute() << std::endl;
    std::cout << "  Is relative: " << std::boolalpha << p.is_relative() << std::endl;
}

int main() {
    namespace fs = std::filesystem; // Common alias

    std::cout << "--- std::filesystem (C++17) --- \n" << std::endl;

    // 1. Basic Path Operations
    std::cout << "1. Basic Path Operations:\n";
    fs::path myPath = "./temp_dir/another_dir/file.txt"; // Can be relative or absolute
    print_path_info(myPath);

    fs::path dirPath = "./test_directory";
    fs::path filePath = dirPath / "example.txt"; // Preferred way to append paths
    std::cout << "\nComposed path: " << filePath.string() << std::endl;

    // 2. Creating and Deleting Directories
    std::cout << "\n2. Creating and Deleting Directories:\n";
    fs::path testDir = "./my_test_dir";
    std::cout << "Creating directory: " << testDir.string() << std::endl;
    if (fs::create_directory(testDir)) {
        std::cout << "  Directory created successfully.\n";
    } else {
        if (fs::exists(testDir) && fs::is_directory(testDir)) {
            std::cout << "  Directory already exists.\n";
        } else {
            std::cout << "  Failed to create directory (or it's a file).\n";
        }
    }
    // Create parent directories if they don't exist
    fs::path nestedDir = testDir / "subdir1" / "subdir2";
    std::cout << "Creating nested directory: " << nestedDir.string() << std::endl;
    if (fs::create_directories(nestedDir)) {
        std::cout << "  Nested directories created successfully.\n";
    } else {
        std::cout << "  Failed to create nested directories (or they exist).\n";
    }

    // Create a dummy file for deletion examples
    std::ofstream dummyFile(nestedDir / "dummy.txt");
    dummyFile << "content";
    dummyFile.close();

    // Deleting a file
    std::cout << "Deleting file: " << (nestedDir / "dummy.txt").string() << std::endl;
    if (fs::remove(nestedDir / "dummy.txt")) {
        std::cout << "  File deleted successfully.\n";
    } else {
        std::cout << "  Failed to delete file.\n";
    }

    // Deleting a directory (must be empty for fs::remove or fs::delete_directory)
    // fs::remove_all recursively deletes a directory and its contents.
    std::cout << "Deleting directory (recursive): " << testDir.string() << std::endl;
    uintmax_t num_deleted = fs::remove_all(testDir);
    std::cout << "  Number of files/directories deleted: " << num_deleted << std::endl;
    if (!fs::exists(testDir)) {
        std::cout << "  Directory " << testDir.string() << " successfully removed.\n";
    } else {
        std::cout << "  Directory " << testDir.string() << " NOT removed.\n";
    }


    // 3. Iterating Directory Contents
    std::cout << "\n3. Iterating Directory Contents (current dir '.'). Errors ignored for simplicity:\n";
    fs::path current_dir = ".";
    std::vector<fs::path> entries;
    try {
        for (const auto& entry : fs::directory_iterator(current_dir)) {
            entries.push_back(entry.path());
        }
        std::sort(entries.begin(), entries.end());
        for (const auto& p : entries) {
            std::cout << "  " << p.filename().string();
            if (fs::is_directory(p)) std::cout << " [DIR]";
            if (fs::is_regular_file(p)) {
                try { std::cout << " (" << fs::file_size(p) << " bytes)"; }
                catch (fs::filesystem_error& e) { std::cout << " (size error)"; }
            }
            std::cout << std::endl;
        }
    } catch (fs::filesystem_error& e) {
        std::cerr << "Filesystem error during iteration: " << e.what() << std::endl;
    }

    // Recursive directory iteration
    // std::cout << "\nRecursive iteration (implementation defined, not shown here for brevity)" << std::endl;
    // for (const auto& entry : fs::recursive_directory_iterator(current_dir, fs::directory_options::skip_permission_denied)) {
    //     // ...
    // }

    // 4. File Properties
    std::cout << "\n4. File Properties (for a newly created file):\n";
    fs::path tempFile = "./temp_example_file.txt";
    std::ofstream(tempFile.string()) << "Hello Filesystem!"; // Create and write to file

    if (fs::exists(tempFile)) {
        std::cout << "File: " << tempFile.string() << std::endl;
        std::cout << "  Is regular file: " << std::boolalpha << fs::is_regular_file(tempFile) << std::endl;
        std::cout << "  Is directory: " << std::boolalpha << fs::is_directory(tempFile) << std::endl;
        try {
            std::cout << "  File size: " << fs::file_size(tempFile) << " bytes" << std::endl;
            auto last_write = fs::last_write_time(tempFile);
            // Converting file_time_type to system_clock::time_point for printing is complex in C++17.
            // C++20 makes this easier with clock_cast.
            // For C++17, it's often platform-specific or involves careful conversion.
            std::cout << "  Last write time: (requires complex conversion to print meaningfully in C++17)" << std::endl;
        } catch (fs::filesystem_error& e) {
            std::cerr << "  Error accessing file properties: " << e.what() << std::endl;
        }
    } else {
        std::cout << "File " << tempFile.string() << " does not exist or is not accessible.\n";
    }
    fs::remove(tempFile); // Clean up


    // 5. Copying, Renaming, Moving (Conceptual - commented out to avoid side effects during automated runs)
    std::cout << "\n5. Copying, Renaming, Moving (Conceptual):\n";
    // fs::path source = "source.txt";
    // fs::path dest = "destination.txt";
    // std::ofstream(source.string()) << "copy test";
    // try {
    //     fs::copy_file(source, dest, fs::copy_options::overwrite_existing);
    //     std::cout << "  Copied " << source.string() << " to " << dest.string() << std::endl;
    //     fs::rename(dest, "renamed.txt");
    //     std::cout << "  Renamed " << dest.string() << " to renamed.txt" << std::endl;
    //     fs::remove(source);
    //     fs::remove("renamed.txt");
    // } catch (fs::filesystem_error& e) {
    //     std::cerr << "  File operation error: " << e.what() << std::endl;
    //     // Clean up potentially created files if error occurred mid-way
    //     if(fs::exists(source)) fs::remove(source);
    //     if(fs::exists(dest)) fs::remove(dest);
    //     if(fs::exists("renamed.txt")) fs::remove("renamed.txt");
    // }
    std::cout << "  (Copy/Rename/Move operations commented out to prevent side-effects in tests).\n";

    std::cout << "\nFilesystem example finished.\n" << std::endl;

    return 0;
}

/*
Explanation:
The `<filesystem>` library, standardized in C++17, provides a platform-independent
way to interact with the file system. It allows you to manipulate paths, directories,
and files, query their properties, and iterate through directory contents.

Key Components:

1.  `std::filesystem::path`:
    -   Represents a path on the file system. It doesn't necessarily mean the path exists.
    -   Provides methods for path decomposition (filename, extension, parent path),
        composition (`operator/` for joining path segments), and modification.
    -   Handles platform-specific path syntaxes (e.g., `/` vs `\`).

2.  Filesystem Operations:
    -   Directory manipulation:
        - `fs::create_directory(path)`: Creates a single directory.
        - `fs::create_directories(path)`: Creates all directories in the path if they don't exist.
        - `fs::remove(path)`: Removes a file or an empty directory.
        - `fs::remove_all(path)`: Removes a path and all its contents recursively (like `rm -rf`).
    -   File operations:
        - `fs::copy_file(from, to, options)`: Copies a file.
        - `fs::rename(from, to)`: Renames or moves a file/directory.
        - `fs::resize_file(path, new_size)`: Changes file size.
    -   Querying status:
        - `fs::exists(path)`: Checks if a path exists.
        - `fs::is_directory(path)`, `fs::is_regular_file(path)`, `fs::is_symlink(path)`, etc.
        - `fs::file_size(path)`: Gets the size of a file.
        - `fs::last_write_time(path)`: Gets/sets the last modification time.
        - `fs::status(path)` and `fs::symlink_status(path)`: Get detailed status information.
    -   Path utility:
        - `fs::current_path()`: Gets/sets the current working directory.
        - `fs::temp_directory_path()`: Gets a path suitable for temporary files.
        - `fs::canonical(path)`, `fs::absolute(path)`: Convert paths.

3.  Directory Iterators:
    -   `fs::directory_iterator(path)`: Iterates over the entries in a directory (non-recursive).
    -   `fs::recursive_directory_iterator(path)`: Iterates recursively through a directory
        and its subdirectories.
    -   Each iterator dereferences to a `fs::directory_entry` object, which provides
        information about a single file system entry (e.g., path, status, type).

Error Handling:
-   Many filesystem functions come in two versions:
    1.  Throwing version: Throws `std::filesystem::filesystem_error` on failure.
        `fs::create_directory(p);`
    2.  Non-throwing version: Takes an `std::error_code&` parameter and reports errors through it.
        `std::error_code ec; fs::create_directory(p, ec); if(ec) { /* handle error */ }`
-   The examples above mostly use the throwing versions for simplicity, sometimes with try-catch.

Namespace Alias:
-   It's common practice to use `namespace fs = std::filesystem;` for brevity.

Important Considerations:
-   Platform Differences: While the library aims for platform independence, some behaviors
    (e.g., symlink handling, permissions, case sensitivity of paths) can still vary.
-   Error Handling: Filesystem operations are prone to errors (permissions, disk full, etc.).
    Robust applications need careful error checking.
-   Time Zones with `last_write_time`: Converting `std::filesystem::file_time_type` to a
    human-readable string, especially considering time zones, is non-trivial in C++17.
    C++20 `std::chrono::clock_cast` simplifies this.

How to compile:
-   g++: `g++ -std=c++17 std_filesystem.cpp -o filesystem_example -lstdc++fs`
    (The `-lstdc++fs` linker flag might be needed for GCC versions before 9, or on some systems.)
-   Clang: `clang++ -std=c++17 std_filesystem.cpp -o filesystem_example -lc++fs`
    (The `-lc++fs` linker flag might be needed for Clang on some systems, especially older ones.)
-   MSVC: `/std:c++17` (usually links automatically).
    Ensure your compiler and standard library fully support C++17 filesystem.
*/
