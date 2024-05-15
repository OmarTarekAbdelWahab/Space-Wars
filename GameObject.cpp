#include "GameObject.h"

GameObject::GameObject(){}
GameObject::GameObject(Sphere sphere): sphere(sphere){}

const Sphere& GameObject::getSphere(){
    return sphere;
}