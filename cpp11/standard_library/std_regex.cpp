// std_regex.cpp
#include <iostream>
#include <string>
#include <regex>    // For std::regex and related functions
#include <vector>

void print_matches(const std::string& text, const std::regex& r, const std::string& description) {
    std::cout << "\n--- " << description << " ---" << std::endl;
    std::cout << "Text: \"" << text << "\"" << std::endl; // MANUALLY CORRECTED QUOTES
    // The regex object itself doesn't store the original pattern string in a directly accessible way post-construction in C++11.
    // We could store the pattern string separately if we wanted to print it.
    std::cout << "Regex pattern: (implementation defined, based on compiled regex object)" << std::endl; 

    std::sregex_iterator it(text.begin(), text.end(), r);
    std::sregex_iterator end; 

    if (it == end) {
        std::cout << "No matches found." << std::endl;
        return;
    }

    std::cout << "Matches found:" << std::endl;
    int match_count = 0;
    for (; it != end; ++it) {
        std::smatch match = *it; 
        match_count++;
        std::cout << "Match " << match_count << ": \"" << match.str() << "\" (position: " << match.position() << ")" << std::endl; // MANUALLY CORRECTED QUOTES
        
        if (match.size() > 1) {
            std::cout << "  Sub-matches:" << std::endl;
            for (size_t i = 1; i < match.size(); ++i) { 
                if (match[i].matched) { 
                    std::cout << "    Group " << i << ": \"" << match[i].str() << "\"" << std::endl; // MANUALLY CORRECTED QUOTES
                } else {
                    std::cout << "    Group " << i << ": (not matched)" << std::endl;
                }
            }
        }
    }
}

int main() {
    // --- 1. std::regex_match ---
    std::cout << "--- std::regex_match ---" << std::endl;
    std::string s1 = "subject";
    std::regex r1("sub.*"); 
    
    if (std::regex_match(s1, r1)) {
        std::cout << "\"" << s1 << "\" matches regex \"sub.*\"" << std::endl; // MANUALLY CORRECTED QUOTES
    } else {
        std::cout << "\"" << s1 << "\" does not match regex \"sub.*\"" << std::endl; // MANUALLY CORRECTED QUOTES
    }

    std::string s2 = "test_subject_test";
    if (std::regex_match(s2, r1)) {
        std::cout << "\"" << s2 << "\" matches regex \"sub.*\"" << std::endl; // MANUALLY CORRECTED QUOTES
    } else {
        std::cout << "\"" << s2 << "\" does not fully match regex \"sub.*\"" << std::endl; // MANUALLY CORRECTED QUOTES
    }
    
    std::string date_str = "Date: 2023-10-26";
    std::regex date_regex("Date: (\\d{4})-(\\d{2})-(\\d{2})"); 
    std::smatch date_match_results; 

    if (std::regex_match(date_str, date_match_results, date_regex)) {
        std::cout << "Full match: " << date_match_results[0].str() << std::endl;
        std::cout << "Year: " << date_match_results[1].str() << std::endl;
        std::cout << "Month: " << date_match_results[2].str() << std::endl;
        std::cout << "Day: " << date_match_results[3].str() << std::endl;
    }

    // --- 2. std::regex_search ---
    std::cout << "\n--- std::regex_search ---" << std::endl;
    std::string search_text = "This is a test string with numbers 123 and 4567.";
    std::regex num_regex("\\d+"); 

    std::smatch search_result;
    if (std::regex_search(search_text, search_result, num_regex)) {
        std::cout << "First number found: \"" << search_result.str() << "\"" << std::endl; // MANUALLY CORRECTED QUOTES
        std::cout << "  Position: " << search_result.position() << std::endl;
        std::cout << "  Prefix before match: \"" << search_result.prefix().str() << "\"" << std::endl; // MANUALLY CORRECTED QUOTES
        std::cout << "  Suffix after match: \"" << search_result.suffix().str() << "\"" << std::endl; // MANUALLY CORRECTED QUOTES
    } else {
        std::cout << "No number found in: \"" << search_text << "\"" << std::endl; // MANUALLY CORRECTED QUOTES
    }

    // --- 3. std::regex_replace ---
    std::cout << "\n--- std::regex_replace ---" << std::endl;
    std::string text_to_replace = "Hello world, hello C++ users.";
    std::regex word_hello("hello");
    std::string replacement = "Greetings";
    
    std::string replaced_text = std::regex_replace(text_to_replace, word_hello, replacement, std::regex_constants::format_sed | std::regex_constants::icase); // Retained format_sed
    std::cout << "Original: \"" << text_to_replace << "\"" << std::endl; // MANUALLY CORRECTED QUOTES
    std::cout << "Replaced (case-insensitive 'hello' with 'Greetings'): \"" << replaced_text << "\"" << std::endl; // MANUALLY CORRECTED QUOTES

    std::string name_swap_text = "LastName, FirstName";
    std::regex name_pattern("(\\w+), (\\w+)"); 
    std::string swapped_format = "$2 $1"; 
    std::string swapped_name = std::regex_replace(name_swap_text, name_pattern, swapped_format);
    std::cout << "Original name: \"" << name_swap_text << "\"" << std::endl; // MANUALLY CORRECTED QUOTES
    std::cout << "Swapped name: \"" << swapped_name << "\"" << std::endl; // MANUALLY CORRECTED QUOTES

    // --- 4. Iterating over all matches (std::sregex_iterator) ---
    std::string iter_text = "apple, pear, orange, apple, banana";
    std::regex fruit_regex("(\\w+)(?:, |$)"); 
    print_matches(iter_text, fruit_regex, "Iterating fruits with capture groups");

    std::string email_text = "Contact us at support@example.com or sales.info@example.co.uk for help.";
    std::regex email_regex(R"([\w.-]+@[\w.-]+\.\w+)"); 
    print_matches(email_text, email_regex, "Finding email addresses");

    // --- 5. Regex Syntax Options (brief mention) ---
    std::cout << "\n(Regex syntax options like std::regex_constants::extended are available but not detailed here.)" << std::endl; // Uncommented as per prompt

    // --- 6. Error Handling ---
    std::cout << "\n--- Regex Error Handling ---" << std::endl;
    try {
        std::regex invalid_regex("[a-z"); 
        std::cout << "This line won't be printed if regex is invalid." << std::endl;
    } catch (const std::regex_error& e) {
        std::cout << "std::regex_error caught:" << std::endl;
        std::cout << "  what(): " << e.what() << std::endl;
        std::cout << "  code(): " << e.code() 
                  << " (relates to std::regex_constants::error_type)" << std::endl;
        if (e.code() == std::regex_constants::error_brack) {
            std::cout << "  Error was 'error_brack' - unbalanced brackets." << std::endl;
        }
    }

    return 0;
}

/*
Explanation:
The `<regex>` library (introduced in C++11) provides support for regular
expressions, allowing for powerful pattern matching in strings.

Core Components:

1.  `std::basic_regex` (and its typedefs `std::regex` for `char`, `std::wregex` for `wchar_t`):
    -   Represents a compiled regular expression.
    -   Constructed from a string pattern.
    -   Can take flags to specify regex grammar (e.g., ECMAScript (default), POSIX, awk)
        and matching behavior (e.g., case-insensitivity `std::regex_constants::icase`).
    -   If the regex pattern is invalid, its constructor throws `std::regex_error`.

2.  Matching Algorithms:
    a.  `std::regex_match(text, regex_obj)`:
        -   Attempts to match the *entire* input `text` against the `regex_obj`.
        -   Returns `true` if the whole text matches, `false` otherwise.
        -   `std::regex_match(text, match_results_obj, regex_obj)`: Overload that
            stores detailed results (including captured groups) in `match_results_obj`
            (e.g., `std::smatch` for string inputs).

    b.  `std::regex_search(text, regex_obj)`:
        -   Searches for the *first subsequence* in `text` that matches `regex_obj`.
        -   Returns `true` if a match is found, `false` otherwise.
        -   `std::regex_search(text, match_results_obj, regex_obj)`: Overload that
            stores results of the first match.

    c.  `std::regex_replace(text, regex_obj, replacement_string, flags)`:
        -   Replaces occurrences of the pattern in `text` with `replacement_string`.
        -   Can use backreferences (e.g., `$1`, `$2`) in `replacement_string` to
            refer to captured groups.
        -   `flags` (e.g., `std::regex_constants::format_default`, `format_sed`)
            control formatting and which parts are replaced.

3.  Match Results (`std::match_results`, e.g., `std::smatch` for strings):
    -   Stores the results of a regex match.
    -   `mr[0]` or `mr.str()`: The entire matched subsequence.
    -   `mr[i]` (for `i > 0`): The i-th captured group (substring matched by
        parentheses in the regex).
    -   `mr.size()`: Number of captured groups + 1 (for the whole match).
    -   `mr.position(i)`: Starting position of the i-th group in the original text.
    -   `mr.length(i)`: Length of the i-th group.
    -   `mr.prefix().str()`: Subsequence before the match.
    -   `mr.suffix().str()`: Subsequence after the match.

4.  Iterators (`std::regex_iterator`, e.g., `std::sregex_iterator`):
    -   Used to find all non-overlapping matches of a regex in a sequence.
    -   `std::sregex_iterator it(text.begin(), text.end(), regex_obj);`
    -   Dereferencing the iterator gives an `std::smatch` object for the current match.
    -   Incrementing the iterator finds the next match.
    -   The default-constructed iterator acts as the end-of-sequence iterator.

5.  `std::regex_token_iterator` (e.g., `std::sregex_token_iterator`):
    -   More advanced; used to iterate over parts of a string that are either
        matches or non-matches, or specific captured groups from matches (e.g.,
        for tokenizing a string). (Not shown in this basic example).

Regex Grammars:
-   The default grammar is ECMAScript (similar to JavaScript regex).
-   Others can be specified via flags like `std::regex_constants::basic`,
    `std::regex_constants::extended`, `std::regex_constants::awk`, `std::regex_constants::grep`.

Error Handling:
-   `std::regex_error` exception is thrown for syntax errors in regex patterns
    or other regex-related operational errors. It contains a `code()` that
    corresponds to `std::regex_constants::error_type` values.

Common Usage:
-   Validating input formats (e.g., emails, phone numbers, dates).
-   Searching for patterns in text.
-   Extracting data from structured text.
-   Replacing text based on patterns.

Performance Note:
-   Compiling a `std::regex` object can be relatively expensive. If a regex is
    used multiple times, it's best to compile it once and reuse the object.

How to compile:
g++ -std=c++11 std_regex.cpp -o std_regex_example
./std_regex_example
*/
