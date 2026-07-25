#pragma once

#include "Point.h"

struct Drawable {
    Point location;
    Point color;
    float radius {};

    Drawable() = default;

    Drawable(float x, float y, float z, float r, float g, float b, float radius)
        : location(x, y, z)
        , color(r, g, b)
        , radius(radius)
    {
    }
};