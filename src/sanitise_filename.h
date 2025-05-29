#include <fmt/base.h>
#include <fmt/ranges.h>
#include <boost/regex.hpp>
#include <string>

class Sanitiser
{
public:
    std::string usage(char** argv);

    std::vector<std::string> split_path(std::string filepath, char sep);

    std::pair<std::string, std::string> extract_extension(std::string filename, std::string sep);

    std::string capitalise(std::string filename);

    std::string sanitise_filename(std::string& filepath, bool NO_EXT, const bool KEEP_CASE);

    void process_args(int argc, char** argv, std::set<int>& skip_args,
            bool& NO_EXT, bool& KEEP_CASE, bool& read_from_stdin, bool& stop_arg_processing);
};
