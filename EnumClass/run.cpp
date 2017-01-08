#include "EnumClass.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>


namespace a {
namespace b {

enum class Color {
    No = -1,
    Red = 0,
    Green,
    Blue,
    Yellow,
    Purple,
    Orange,
    Begin = Red,
    End = Orange
};

}
}

void changeEnum(a::b::Color& literal)
{
    literal = a::b::Color::Purple;
}


int main()
{
    using Color = a::b::Color;

    // Default complete enumeration.
    for (auto iter : Enum<Color>()) {
        std::cout << static_cast<int>(iter) << std::endl;
    }
    std::cout << std::endl;

    // Customize the enumeration range.
    for (auto iter : Enum<Color, Color::Blue, Color::Purple>()) {
        std::cout << static_cast<int>(iter) << std::endl;
    }
    std::cout << std::endl;

    auto literal = Color::Red;
    std::cout << "before " << static_cast<int>(literal) << std::endl;
    changeEnum(literal);
    std::cout << "after  " << static_cast<int>(literal) << std::endl;

    return 0;
}