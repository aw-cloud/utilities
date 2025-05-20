#include <fmt/base.h>
#include <fmt/ranges.h>
#include <iostream>
#include <boost/regex.hpp>
#include <unordered_set>
#include <string>

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

    // check if the filename already starts with an underscore
    // if it does, we will restore it later
    bool starts_with_underscore { filename[0] == '_' };

    // any of these chars will be removed without checking to see if they are
    // needed to match later regexes
    static std::unordered_set<wchar_t> disallowed_chars_pre
    {
        '`','\'', L'‘', L'’', L'‛', L'‚',
        '~', '!', '"', '$', '%', '^', '*', '(', ')', '-', '=', ']', '[',
        '}', '.', '{', '\'', '\'', '~', '\'', '@', ';', ':', '/', '\\',
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


    if (!no_ext)
        filename += '.' +  extension;
    fmt::print("{:s}\n", fmt::join(filepath_split, "/"));
    return EXIT_SUCCESS;
}
