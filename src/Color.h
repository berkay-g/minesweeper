#pragma once

#include "SDL3/SDL_stdinc.h"

class Color
{
public:
    Color();

    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Magenta;
    static const Color Cyan;

    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
};

bool operator==(const Color &left, const Color &right);
bool operator!=(const Color &left, const Color &right);