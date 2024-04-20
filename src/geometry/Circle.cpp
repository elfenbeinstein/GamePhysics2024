#include "Circle.h"

Circle::Circle() : Position(0, 0), Velocity(0, 0), Radius(0.5f) {}

Circle::Circle(glm::vec2 pos, glm::vec2 vel, float rad)
    : Position(pos), Velocity(vel), Radius(rad) {}
