#include <fmt/core.h>
#include <fmt/ranges.h>
#include <iostream>
#include <boost/regex.hpp>
#include <unordered_set>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "no filename provided\n";
        return EXIT_FAILURE;
    }
    std::string filepath { argv[1] };
    // split filepath into chunks for each dir
    std::vector<std::string> filepath_split {};
    auto left { filepath.begin() };
    auto it_end { filepath.end() };
    auto right = it_end;

    while ((right = std::find(left, it_end, '/')) != it_end) {
        filepath_split.push_back({left, right});
        left = right + 1;
    }
    filepath_split.push_back({left, right});


    // choose the last chunk as the filename
    std::string& filename {*(filepath_split.end()-1)};
    fmt::print("{:s}\n", fmt::join(filepath_split, "/"));
    return EXIT_SUCCESS;
}
