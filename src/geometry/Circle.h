#pragma once
#include "core/Simple2DScene.h"
#include "core/Colors.h"

class Circle {
public:
    Circle();
    Circle(glm::vec2 pos, glm::vec2 vel, float rad);
    Circle::Circle(glm::vec2 pos, glm::vec2 vel, float rad, float mass);

    glm::vec2 Position;
    glm::vec2 Velocity;
    glm::vec2 Acceleration;
    float Radius;
    float Mass;
    glm::vec2 TotalForce;
    Color Color;
    float ColorCountdown;
};
