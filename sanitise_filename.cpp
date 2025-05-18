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
    bool no_ext {false};
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

    // extract the file extension
    std::string extension {};
    auto ext_pos { filename.rfind('.', filename.size()-1) };
    if (ext_pos == filename.npos)
        no_ext = true;
    else
        extension = filename.substr(ext_pos+1, filename.size() -1);
    // remove file extension from the filename
    filename = filename.substr(0, ext_pos);
    if (!no_ext)
        filename += '.' +  extension;
    fmt::print("{:s}\n", fmt::join(filepath_split, "/"));
    return EXIT_SUCCESS;
}
