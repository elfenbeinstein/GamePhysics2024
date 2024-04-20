#pragma once
#include "core/Simple2DScene.h"

class Circle {
public:
    Circle();
    Circle(glm::vec2 pos, glm::vec2 vel, float rad);

    glm::vec2 Position;
    glm::vec2 Velocity;
    float Radius;
};
