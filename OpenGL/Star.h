#pragma once

#include "Drawable.h"

struct Star : public Drawable {

    Star() = default;

    Star(float x, float y, float z, float r, float g, float b, float radius)
        : Drawable(x, y, z, r, g, b, 1.3 * radius)
    {
    }
};