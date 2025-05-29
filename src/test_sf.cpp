#include "gmock/gmock.h"
#include <fstream>
#include "sanitise_filename.h"
#include <json/json.h>

using namespace testing;

class UtilitiesTest: public Test
{
protected:

public:
    Sanitiser sanitiser;
};

TEST_F(UtilitiesTest, original) {
    std::ifstream is { "../tests/filenames.txt" };
    std::string filename {};
    while (getline(is, filename)) {
        std::string actual { sanitiser.sanitise_filename(filename, false, false) };
        std::string expected {};
        if (!getline(is, expected))
            throw std::runtime_error("Incomplete test case: missing expected output");

        ASSERT_THAT(actual, Eq(expected));
    }
}

