#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <ranges>
#include <utility>
#include <algorithm>

#include "Galaxy.h"

constexpr const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in float aPointSize;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   gl_PointSize = aPointSize;\n"
"   ourColor = aColor;\n"
"}\0";

constexpr const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"    vec2 p = gl_PointCoord * 2.0 - 1.0;\n"
"    float d = length(p);\n"
"\n"
"    float alpha = 1.0 - smoothstep(0.9, 1.0, d);\n"
"\n"
"    if (alpha <= 0.0)\n"
"        discard;\n"
"    FragColor = vec4(ourColor, alpha);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {

    constexpr int WIDTH = 800;
    constexpr int HEIGHT = 600;

    unsigned int VAO, VBO;
    int  success;
    char infoLog[512];

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(
        800, 600,
        "Ceu estrelado",
        nullptr,
        nullptr
    );

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_PROGRAM_POINT_SIZE);

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    auto shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

	double startTime = glfwGetTime() - 3.0;

	std::vector<Galaxy> galaxies;
    std::vector<Drawable> drawables;

    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

    constexpr int NUMBER_OF_GALAXIES = 30;
	constexpr int NUMBER_OF_STARS_PER_GALAXY = 30;
	constexpr int NUMBER_OF_PLANETS_PER_STAR = 150;

    auto paintGalaxies = [&] {
        drawables.clear();

        std::ranges::for_each(galaxies, [&](const Galaxy& galaxy) {
            drawables.insert(drawables.end(), galaxy.stars.begin(), galaxy.stars.end());
            drawables.insert(drawables.end(), galaxy.planets.begin(), galaxy.planets.end());
         });

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Drawable) * drawables.size(), drawables.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    };


    auto uniform = [&rng](float min, float max) {
        return std::uniform_real_distribution<float>(min, max)(rng);
	};  


    float step = 2.0f / NUMBER_OF_GALAXIES;

    std::vector<std::pair<float, float>> xRanges, yRanges, zRanges;

    for (int i = 0; i < NUMBER_OF_GALAXIES; ++i) {
        xRanges.emplace_back(-1.0f + step * i, -1.0f + step * (i + 1));
        yRanges.emplace_back(-1.0f + step * i, -1.0f + step * (i + 1));
        zRanges.emplace_back(-1.0f + step * i, -1.0f + step * (i + 1));
    }

    std::shuffle(xRanges.begin(), xRanges.end(), rng);
    std::shuffle(yRanges.begin(), yRanges.end(), rng);
    std::shuffle(zRanges.begin(), zRanges.end(), rng);

    for (const auto&& [xRange, yRange, zRange] : std::views::zip(xRanges, yRanges, zRanges)) {
        auto [xL, xR] = xRange;
        auto [yL, yR] = yRange;
        auto [zL, zR] = zRange;

        float x = uniform(xL, xR);
        float y = uniform(yL, yR);
        float z = uniform(zL, zR);

        galaxies.emplace_back(x, y, z);

        auto& galaxy = galaxies.back();

        for (int j = 0; j < NUMBER_OF_STARS_PER_GALAXY; ++j) {
            float x = uniform(-1.0f, 1.0f);
            float y = uniform(-1.0f, 1.0f);
            float z = uniform(-1.0f, 1.0f);
            float r = uniform(0.0f, 1.0f);
            float g = uniform(0.0f, 1.0f);
            float b = uniform(0.0f, 1.0f);
            float pointSize = uniform(0.5f, 4.5f);

            Star star(x, y, z, r, g, b, pointSize);

            galaxy.addStar(star);

            for (int k = 0; k < NUMBER_OF_PLANETS_PER_STAR; ++k) {
                float x = uniform(-1.0f, 1.0f);
                float y = uniform(-1.0f, 1.0f);
                float z = uniform(-1.0f, 1.0f);
                float r = uniform(0.0f, 1.0f);
                float g = uniform(0.0f, 1.0f);
                float b = uniform(0.0f, 1.0f);
                float pointSize = uniform(0.3f, 1.0f);

                Planet planet(x, y, z, r, g, b, pointSize, star);

                galaxy.addPlanet(planet);
            }
        }
    }
    
	constexpr double ROTATION_DELAY = 3.0;

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.08f, 0.04f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

		double currentTime = glfwGetTime();

       if (currentTime - startTime >= ROTATION_DELAY) {
			startTime = currentTime;

            std::ranges::for_each(galaxies, [](Galaxy& galaxy) {
                galaxy.rotate();
		    });

			paintGalaxies();
        }

        glUseProgram(shaderProgram);
        glDrawArrays(GL_POINTS, 0, drawables.size());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}