#pragma once

#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "Star.h"
#include "Planet.h"

struct Galaxy {
    std::vector<Star> stars;
    std::vector<Planet> planets;
    Point center;
    float rotationSpeed = 0.01f;

    Galaxy() = default;

    Galaxy(float x, float y, float z)
        : center(x, y, z)
    {
        std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
        rotationSpeed = std::uniform_real_distribution<float>(0.001f, 0.01f)(rng);
    }

    void addStar(Star& star) {
		auto norm = glm::normalize(star.location - center);

		star.location = center + norm * 0.2f;

        stars.push_back(star);
    }

    void addPlanet(Planet& planet) {
		const Star& star = planet.star;

		auto norm = glm::normalize(planet.location - star.location);

		planet.location = star.location + norm * 0.1f;

        planets.push_back(planet);
    }

    void rotate() {

        for (auto& planet : planets) {
            float dx = planet.location.x - center.x;
            float dy = planet.location.y - center.y;

            planet.location.x = dx * std::cos(rotationSpeed) - dy * std::sin(rotationSpeed) + center.x;
            planet.location.y = dx * std::sin(rotationSpeed) + dy * std::cos(rotationSpeed) + center.y;
        }

        for(auto& start : stars) {
            float dx = start.location.x - center.x;
            float dy = start.location.y - center.y;

            start.location.x = dx * std::cos(rotationSpeed) - dy * std::sin(rotationSpeed) + center.x;
            start.location.y = dx * std::sin(rotationSpeed) + dy * std::cos(rotationSpeed) + center.y;
		}
    }
};