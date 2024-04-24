#include "Circle.h"

Circle::Circle()
    : Position(0, 0), Velocity(0, 0), Acceleration(0, 0), Radius(0.5f), Mass(1.0f), TotalForce(0, 0) {}

Circle::Circle(glm::vec2 pos, glm::vec2 vel, float rad)
    : Position(pos), Velocity(vel), Acceleration(0, 0), Radius(rad), Mass(1.0f), TotalForce(0, 0) {}

Circle::Circle(glm::vec2 pos, glm::vec2 vel, float rad, float mass) 
    : Position(pos), Velocity(vel), Acceleration(0, 0), Radius(rad), Mass(mass), TotalForce(0, 0) {}