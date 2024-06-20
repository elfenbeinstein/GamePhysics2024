#pragma once
#include "core/Simple2DScene.h"
#include "geometry/Particle.h"

class Line : public Particle {
public:
    Line();
    Line(glm::vec2 start, glm::vec2 end);
    Line::Line(glm::vec2 start, glm::vec2 end, bool deleteCollidingCircle, bool canBeRemoved);

    glm::vec2 EndOffset;
    glm::vec2 Direction;
    glm::vec2 Normal;
    float ProjectionOntoNormal;

    void Draw() override;

private:
    void Line::CalculateNormal();
};
