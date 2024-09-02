#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include "json/json.h"
#include <cppbind/JsonDecoder.hpp>
#include <cppbind/JsonEncoder.hpp>
#include <cppbind/csvbind.hpp>
#include "cppbind/object/ObjFactoryT.h"
#include <cppbind/converter/timestr.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <cppbind/util/EnumHelper.hpp>
#include "cppbind/dynamic/ClassRegister.h"
#include "cppbind/datatype/enum.h"

using namespace boost::posix_time;
using namespace cppbind;
using namespace std;

#if 0

class Color {
public:
    enum class Enum : uint32_t {
        Red, Yellow, Green,
    };
    static constexpr Enum Red = Enum::Red;
    static constexpr Enum Yellow = Enum::Yellow;
    static constexpr Enum Green = Enum::Green;

    explicit Color(Enum v) : value(v) {}

    Enum getValue() const { return value; }

    uint32_t toUnderlying() const { return static_cast<uint32_t>(value); }

    static std::string toStr(Enum v) {
        switch (v) {
            case Enum::Red:
                return "Red";
            case Enum::Yellow:
                return "Yellow";
            case Enum::Green:
                return "Green";
            default:
                return "Unknown";
        }
    }

private:
    Enum value;
};

#else

CPPBIND_ENUM(Color, uint32_t,
             (Red)
             (Yellow)
             (Green)
)

#endif

TEST(enum_test, decode) {
    Color color = Color(Color::Green);
    ASSERT_EQ(color.toStr() ,"Green");
    ASSERT_EQ(color.toUnderlying(), 2);

    color = Color("Red");
    ASSERT_EQ(color.toUnderlying(), 0);


    color = Color("red");
    ASSERT_EQ(color.toStr(), "Unknown");


};

