#include "Color.h"

Color::Color()
    : r(0), g(0), b(0), a(255)
{
}

Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    : r(r), g(g), b(b), a(a)
{
}

const Color Color::Black(0, 0, 0);
const Color Color::White(255, 255, 255);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::Magenta(255, 0, 255);
const Color Color::Cyan(0, 255, 255);

bool operator==(const Color &left, const Color &right)
{
    return (left.r == right.r) && (left.g == right.g) && (left.b == right.b) && (left.a == right.a);
}

bool operator!=(const Color &left, const Color &right)
{
    return !(left == right);
}