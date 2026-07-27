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
    double rotationSpeed = 1.0;

    Galaxy() = default;

    Galaxy(float x, float y, float z)
        : center(x, y, z)
    {
        std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
        rotationSpeed = std::uniform_real_distribution<double>(1.0, 5.0)(rng);
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

    void rotate(double speed) {
		
		double rotationSpeed = this->rotationSpeed * speed;
        std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

        for (auto& planet : planets) {
			auto delta = planet.location - center;

            planet.location.x = delta.x * std::cos(glm::radians(rotationSpeed)) - delta.y * std::sin(glm::radians(rotationSpeed)) + center.x;
            planet.location.y = delta.x * std::sin(glm::radians(rotationSpeed)) + delta.y * std::cos(glm::radians(rotationSpeed)) + center.y;
        }

        for(auto& star : stars) {
			auto delta = star.location - center;

            star.location.x = delta.x * std::cos(glm::radians(rotationSpeed)) - delta.y * std::sin(glm::radians(rotationSpeed)) + center.x;
            star.location.y = delta.x * std::sin(glm::radians(rotationSpeed)) + delta.y * std::cos(glm::radians(rotationSpeed)) + center.y;

			star.frequency = std::uniform_real_distribution<float>(0.5f, 1.0f)(rng);
		}
    }
};