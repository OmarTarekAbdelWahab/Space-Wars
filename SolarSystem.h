#include <iostream>
#include <vector>
#include "Planet.h"

#ifndef SOLARSYSTEM_H_
#define SOLARSYSTEM_H_

class SolarSystem{
private:
    std::vector<Planet> planets;
public:
    SolarSystem();
    void addPlanet(Planet planet);
    void update();
    void draw();
    std::vector<Planet>& getPlanets(void);
};

#endif