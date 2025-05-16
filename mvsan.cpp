#include <string>
#include <format>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "no filename provided\n";
        return 1;
    }
    std::string filename { argv[1] };
    std::cout << std::format("original filename: \"{}\"\n", filename);
}
