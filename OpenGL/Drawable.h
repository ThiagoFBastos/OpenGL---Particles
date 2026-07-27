#pragma once

#include "Point.h"

struct Drawable {
    Point location;
    Point color;
    float radius {};
	float frequency;

    Drawable() = default;

    Drawable(float x, float y, float z, float r, float g, float b, float radius, float frequency = 1.0f)
        : location(x, y, z)
        , color(r, g, b)
        , radius(radius)
        , frequency(frequency)
    {
    }
};