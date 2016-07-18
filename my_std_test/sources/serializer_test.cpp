#include "catch.hpp"
#include "json_serializer.hpp"
#include <list>

using namespace std;
using namespace my_std;

struct dummy
{
    dummy(int val = 0)
        : value(val)
    {}

    int value;
};

ostream& operator<<(ostream& out, const dummy& d)
{
    return out << "dummy " << d.value;
}

istream& operator >> (istream& in, dummy& d)
{
    string s;

    if (!(in >> s >> d.value) || s != "dummy")
        in.setstate(ios_base::failbit);
    return in;
}

struct serializable
{
    string toto;
    int value;

    serializable(const string& t = "", int v = 0) : toto(t), value(v) {}

    template<typename TSerializer>
    void serialize(TSerializer& s) const
    {
        put(s, "toto", toto);
        put(s, "value", value);
    }

    template<typename TSerializer>
    bool deserialize(TSerializer& s)
    {
        return
            get(s, "toto", toto) &&
            get(s, "value", value);
    }
};

TEST_CASE("Json serializer", "[serial][json]")
{
    SECTION("Basic")
    {
        json_serializer js;

        put(js, "int", 13);
        put(js, "float", 13.2f);
        put(js, "double", 18.89);
        put(js, "string", string("str"));
        put(js, "bool", true);
        put(js, "dummy", dummy{ 42 });
        put(js, "array", array<int, 4>{ 1, 2, 3, 4 });
        put(js, "container", vector<dummy>{ 8, 7, 6, 5 });
        put(js, "serializable", serializable("toto", 42));

        int i;
        REQUIRE(get(js, "int", i)); REQUIRE(i == 13);

        string s;
        REQUIRE(get(js, "string", s)); REQUIRE(s == "str");

        dummy d;
        REQUIRE(get(js, "dummy", d)); REQUIRE(d.value == 42);

        int a[4];
        REQUIRE(get(js, "array", a)); REQUIRE(a[0] == 1); REQUIRE(a[1] == 2); REQUIRE(a[2] == 3); REQUIRE(a[3] == 4);

        list<dummy> c;
        REQUIRE(get(js, "container", c)); REQUIRE(c.size() == 4);

        serializable se;
        REQUIRE(get(js, "serializable", se)); REQUIRE(se.toto == "toto"); REQUIRE(se.value == 42);

        string dump = js.dump();
        INFO(dump);
    }
}