#include <fmt/base.h>
#include <fmt/ranges.h>
#include <iostream>
#include <boost/regex.hpp>
#include <unordered_set>
#include <string>

std::string usage(char** argv)
{ return std::format("usage: {} [-n|-k] <filepath> [filepath2 ... ]\n", argv[0]); }

int main(int argc, char** argv)
{
    bool NO_EXT { false };
    bool KEEP_CASE { false };

    if (argc < 2) {
        std::cerr << "no filename provided\n";
        std::cerr << usage(argv);
        return EXIT_FAILURE;
    }

    // process args for flags and options
    // indices of args are stored so they aren't processed as input later
    std::set<int> skip_args { 0 };
    try {
        for (int arg = 0; arg != argc; ++arg) {
            std::string arg_s { argv[arg] };
            // an argument of "--" stops further scanning for arguments
            if (arg_s == "--") {
                skip_args.insert(arg);
                break;
            }
            // skip input that isn't a flag or option
            if (!arg_s.starts_with("-"))
                    continue;
            else {
                // trim arg until string is consumed or error occurs
                // allows for combined flags like -nk
                while ((arg_s = arg_s.substr(1,arg_s.size())) != "") {
                    if (arg_s.starts_with("n")) {
                        NO_EXT = true;
                        skip_args.insert(arg);
                        continue;
                    }
                    if (arg_s.starts_with("k")) {
                        KEEP_CASE = true;
                        skip_args.insert(arg);
                        continue;
                    }
                    throw std::runtime_error(std::string("unknown argument: " + arg_s));
                }
            }
        }
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        std::cerr <<
            "try passing \"--\" as the first argument to avoid accidentally treating input as a flag or option\n";
        return EXIT_FAILURE;
    }

    std::vector<std::string> filepaths {};
    for (int arg = 0; arg != argc; ++arg) {
        if (!skip_args.contains(arg))
            filepaths.push_back(argv[arg]);
    }

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

        std::string extension {};
        std::string ext_sep {};
        if (!NO_EXT) {
            ext_sep = ".";
            // extract the file extension
            auto ext_pos { filename.rfind('.', filename.size()-1) };
            if (ext_pos == std::string::npos)
                ext_sep = "";
            else {
                extension = filename.substr(ext_pos+1, filename.size() -1);
            }
            // remove file extension from the filename
            filename = filename.substr(0, ext_pos);
        }

        // check if the filename already starts with an underscore
        // if it does, we will restore it later
        bool starts_with_underscore { filename[0] == '_' };

        // any of these chars will be removed without checking to see if they are
        // needed to match later regexes
        static std::unordered_set<wchar_t> disallowed_chars_pre
        {
            '`','\'', L'‘', L'’', L'‛', L'‚',
            '~', '!', '"', '$', '%', '^', '*', '(', ')', '-', '=', ']', '[',
            '}', '.', '{', '\'', '\'', '~', '\'', '@', ';', ':', '/', '\\', ',',
            ' ', L'£', L'¬'
        };

        // these disallowed chars are removed after the other regexes
        // only chars that are needed for regex patterns above should live here
        static std::unordered_set<wchar_t> disallowed_chars_post
        { '&', '#', '+' };

        // remove chars
        for (auto& c : filename)
            if (disallowed_chars_pre.contains(c))
                c = '_';

    boost::regex ascii_only ("[^\\x20-\\x7E]");
    boost::regex underscores ("_+");

    filename = boost::regex_replace(filename, ascii_only, "_");
    filename = boost::regex_replace(filename, underscores, "_");

    boost::regex cpp { "[c,C]\\+\\+" };
    filename = boost::regex_replace(filename, cpp, "CPP");

    boost::regex c_sharp { "[c,C]#" };
    filename = boost::regex_replace(filename, c_sharp, "C_Sharp");

    boost::regex ampersand { "&" };
    filename = boost::regex_replace(filename, ampersand, "_and_");
    // trailing underscores
    filename = boost::regex_replace(filename, boost::regex {"_+$"}, "");

    // remove chars
    for (auto& c : filename)
        if (disallowed_chars_post.contains(c))
            c = '_';

    filename = boost::regex_replace(filename, underscores, "_");

    // remove prefix underscore unless it was part of the original input
    if (!starts_with_underscore && filename[0] == '_' && filename.size() > 1)
        filename = filename.substr(1,filename.size());

    // capitalisation

    if (!KEEP_CASE) {
        auto fb { filename.cbegin() };
        auto fe { filename.cend() };
        boost::match_results<std::string::const_iterator> match;
        boost::regex word_start ("(?<=_)(?!((as|of|and|at|in|on|the|which|or|nor|so|yet|up|per|via|for|are|with|it|by|to|from|that|is)_))([a-z])");

    std::string temp {};
    while (boost::regex_search(fb, fe, match, word_start)) {
        temp.append(fb, match[0].first);
        temp += std::toupper(match[0].str()[0]);
        fb = match[0].second;
    }
    temp.append(fb, fe);
    filename = temp;

        boost::regex_search(filename.cbegin(), filename.cend(), match, boost::regex {"^_([a-z])"});
        auto target = std::distance(filename.cbegin(), match[1].first);
        filename[target] = toupper(filename[target]);
        filename[0] = toupper(filename[0]);
    }

    filename += ext_sep +  extension;
    fmt::print("{:s}\n", fmt::join(filepath_split, "/"));
    return EXIT_SUCCESS;
}
