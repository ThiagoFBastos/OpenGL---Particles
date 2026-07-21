#pragma once

#include "Drawable.h"
#include "Star.h"

struct Planet : public Drawable {
    Star star;

    Planet() = default;

    Planet(float x, float y, float z, float r, float g, float b, float radius, const Star& star)
        : Drawable(x, y, z, r, g, b, radius)
        , star(star)
    {
    }
};