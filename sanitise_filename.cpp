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
    std::string filename { argv[1] };
    std::cout << std::format("original filename: \"{}\"\n", filename);
    return EXIT_SUCCESS;
}
