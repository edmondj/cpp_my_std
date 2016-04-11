#include "file_system.hpp"
#include "catch.hpp"

using namespace std;
using namespace my_std;

TEST_CASE("Testing file system", "[file_system]")
{
    SECTION("File extension")
    {
        REQUIRE(fs::get_file_extension("/usr/tmp.d/") == "");
        REQUIRE(fs::get_file_extension("/usr/tmp.d/toto") == "");
        REQUIRE(fs::get_file_extension("/usr/tmp.d/toto.xml") == "xml");
    }
}