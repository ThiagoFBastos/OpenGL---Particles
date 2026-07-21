#pragma once

#include <vector>
#include <cmath>
#include <random>
#include <chrono>

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
        float distance = std::sqrt(std::pow(star.location.x - center.x, 2) +
            std::pow(star.location.y - center.y, 2) +
            std::pow(star.location.z - center.z, 2));

        star.location.x = center.x + (star.location.x - center.x) * (1.0f / distance) * 0.2;
        star.location.y = center.y + (star.location.y - center.y) * (1.0f / distance) * 0.2;
        star.location.z = center.z + (star.location.z - center.z) * (1.0f / distance) * 0.2;

        stars.push_back(star);
    }

    void addPlanet(Planet& planet) {
		const Star& star = planet.star;

        float distance = std::sqrt(std::pow(planet.location.x - star.location.x, 2) +
            std::pow(planet.location.y - star.location.y, 2) +
            std::pow(planet.location.z - star.location.z, 2));

        planet.location.x = star.location.x + (planet.location.x - star.location.x) * (1.0f / distance) * 0.1;
        planet.location.y = star.location.y + (planet.location.y - star.location.y) * (1.0f / distance) * 0.1;
        planet.location.z = star.location.z + (planet.location.z - star.location.z) * (1.0f / distance) * 0.1;

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