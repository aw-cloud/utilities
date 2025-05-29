#include "sanitise_filename.h"
#include <iostream>
int main(int argc, char** argv)
{
    Sanitiser sanitiser;
    if (argc < 2) {
        std::cerr << "no filename provided\n";
        std::cerr << sanitiser.usage(argv);
        return EXIT_FAILURE;
    }

    bool NO_EXT { false };
    bool KEEP_CASE { false };
    bool read_from_stdin { false };
    bool stop_arg_processing { false };

    // process args for flags and options
    // indices of args are stored so they aren't processed as input later
    std::set<int> skip_args { 0 };
    try {
        sanitiser.process_args(argc, argv, skip_args, NO_EXT, KEEP_CASE,
                read_from_stdin, stop_arg_processing);
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        static const std::string tip { "pass \"-\" or \"--\" as the first "
            "argument to process input starting with a \"-\"\n"
        };
        std::cerr << tip;
        return EXIT_FAILURE;
    }

    std::vector<std::string> filepaths {};

    // read from argv
    for (int arg { 1 }; arg != argc; ++arg)
        if (!skip_args.contains(arg))
            filepaths.push_back(argv[arg]);

    if (!read_from_stdin && filepaths.size() == 0) {
        if (stop_arg_processing)
            read_from_stdin = true;
        else {
            std::cerr << "no filename provided\n";
            std::cerr << sanitiser.usage(argv);
            return EXIT_FAILURE;
        }
    }

    // read from stdin
    if (read_from_stdin) {
        std::string s {};
        while (getline(std::cin, s, '\n'))
            filepaths.push_back(s);
    }

    for (auto filepath : filepaths)
        sanitiser.sanitise_filename(filepath, NO_EXT, KEEP_CASE);

    return EXIT_SUCCESS;
}
