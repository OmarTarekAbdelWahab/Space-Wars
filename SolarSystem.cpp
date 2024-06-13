#include "SolarSystem.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <vector>
#include <random>
SolarSystem::SolarSystem(){
    // std::cout << "Created";
}
void SolarSystem::populate(){
    // Sun
    planets.emplace_back(std::vector<GLfloat>{1.0, 1.0, 0.0}, 0., 0.005, 0.5, 360.0*rand()/RAND_MAX, 0.5, Sphere(0., 0., 0., 80.));
    // mercury
    planets.emplace_back(std::vector<GLfloat>{0.4, 0.0, 0.0}, 150.0, 0.5, 0.5, 360.0*rand()/RAND_MAX, 30.0, Sphere(0., 0., 0., 16.));
    // venus
    planets.emplace_back(std::vector<GLfloat>{0.6, 0.0, 0.298}, 200.0, 0.4, 0.5, 360.0*rand()/RAND_MAX, 30.0, Sphere(0., 0., 0., 28.));
    // earth
    planets.emplace_back(std::vector<GLfloat>{0.0, 0.0, 1.0}, 270.0, 0.2, 0.5, 360.0*rand()/RAND_MAX, 30.0, Sphere(0., 0., 0., 40.));
    // mars
    planets.emplace_back(std::vector<GLfloat>{0.6, 0.0, 0.298}, 400.0, 0.2, 0.5, 360.0*rand()/RAND_MAX, 30.0, Sphere(0., 0., 0., 24.));
    // jupiter
    planets.emplace_back(std::vector<GLfloat>{0.6, 0.0, 0.0}, 550.0, 0.1, 0.5, 360.0*rand()/RAND_MAX, 30.0, Sphere(0., 0., 0., 70.));
    // saturn
    planets.emplace_back(std::vector<GLfloat>{1.0, 0.6, 0.2}, 750.0, 0.2, 0.5, 360.0*rand()/RAND_MAX, 30.0, Sphere(0., 0., 0., 48.));
    // uranus
    planets.emplace_back(std::vector<GLfloat>{0.0, 0.0, 0.3}, 850.0, 0.3, 0.5, 360.0*rand()/RAND_MAX, 30.0, Sphere(0., 0., 0., 40.));
    // neptune
    planets.emplace_back(std::vector<GLfloat>{0.0, 0.0, 0.6}, 1000.0, 0.2, 0.5, 360.0*rand()/RAND_MAX, 30.0, Sphere(0., 0., 0., 40.));
}
std::vector<Planet>& SolarSystem::getPlanets(void){
    return planets;
}
SolarSystem::~SolarSystem(){}
void SolarSystem::addPlanet(Planet planet){
    planets.emplace_back(planet);
}
void SolarSystem::update(void){
    for(Planet& planet: planets){
        planet.update();
    }
}
void SolarSystem::draw(void){
    for(Planet& planet: planets) {
        planet.draw();
    }
}
