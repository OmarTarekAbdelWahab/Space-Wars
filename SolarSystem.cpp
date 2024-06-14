#include "SolarSystem.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <random>
SolarSystem::SolarSystem()
{
    // std::cout << "Created";
}
void SolarSystem::populate(unsigned int textures[])
{
    // Sun
    planets.emplace_back(std::vector<GLfloat>{1.0, 1.0, 0.0}, 0., 0.005, 0.5, 360.0 * rand() / RAND_MAX, 0.5, Sphere(0., 0., 0., 80.), textures[0]);
    // mercury
    planets.emplace_back(std::vector<GLfloat>{0.4, 0.0, 0.0}, 150.0, 0.5, 0.5, 360.0 * rand() / RAND_MAX, 30.0, Sphere(0., 0., 0., 16.), textures[1]);
    // venus
    planets.emplace_back(std::vector<GLfloat>{0.6, 0.0, 0.298}, 200.0, 0.4, 0.5, 360.0 * rand() / RAND_MAX, 30.0, Sphere(0., 0., 0., 28.), textures[2]);
    // earth
    planets.emplace_back(std::vector<GLfloat>{0.0, 0.0, 1.0}, 270.0, 0.2, 0.5, 360.0 * rand() / RAND_MAX, 30.0, Sphere(0., 0., 0., 40.), textures[3]);
    // mars
    planets.emplace_back(std::vector<GLfloat>{0.6, 0.0, 0.298}, 400.0, 0.2, 0.5, 360.0 * rand() / RAND_MAX, 30.0, Sphere(0., 0., 0., 24.), textures[4]);
    // jupiter
    planets.emplace_back(std::vector<GLfloat>{0.6, 0.0, 0.0}, 550.0, 0.1, 0.5, 360.0 * rand() / RAND_MAX, 30.0, Sphere(0., 0., 0., 70.), textures[5]);
    // saturn
    planets.emplace_back(std::vector<GLfloat>{1.0, 0.6, 0.2}, 750.0, 0.2, 0.5, 360.0 * rand() / RAND_MAX, 30.0, Sphere(0., 0., 0., 48.), textures[6]);
    // uranus
    planets.emplace_back(std::vector<GLfloat>{0.0, 0.0, 0.3}, 850.0, 0.3, 0.5, 360.0 * rand() / RAND_MAX, 30.0, Sphere(0., 0., 0., 40.), textures[7]);
    // neptune
    planets.emplace_back(std::vector<GLfloat>{0.0, 0.0, 0.6}, 1000.0, 0.2, 0.5, 360.0 * rand() / RAND_MAX, 30.0, Sphere(0., 0., 0., 40.), textures[8]);
}
std::vector<Planet> &SolarSystem::getPlanets(void)
{
    return planets;
}
SolarSystem::~SolarSystem() {}
void SolarSystem::addPlanet(Planet planet)
{
    planets.emplace_back(planet);
}
void SolarSystem::update(void)
{
    for (Planet &planet : planets)
    {
        planet.update();
    }
}
void SolarSystem::draw(void)
{
    for (Planet &planet : planets)
    {
        planet.draw();
    }
}
