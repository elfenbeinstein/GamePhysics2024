#pragma once
#include "core/Simple2DScene.h"
#include "physics/Particle.h"

class Line : public Particle {
public:
    Line();
    Line::Line(glm::vec2 start,
               glm::vec2 end,
               bool deleteCollidingParticle,
               bool canBeRemoved);
    Line::Line(glm::vec2 start,
               glm::vec2 end,
               float mass = 0.0f,
               float damping = 0.0f,
               float coefficientOfRestitution = 1.0f,
               glm::vec2 vel = {0.0f, 0.0f},
               glm::vec2 totalForce = {0.0f, 0.0f},
               glm::vec2 position = {0.0f, 0.0f},
               bool removeAfterCol = false,
               bool deleteCollidingParticle = false,
               bool canBeRemoved = false,
               bool canBeMoved = false,
               bool canAddImpulse = false,
               Color color = Colors::blue);

    glm::vec2 End;
    glm::vec2 Direction;
    glm::vec2 Normal;
    float ProjectionOntoNormal;

    void Draw() override;
    bool IsMouseOverParticle(glm::vec2 mousePosition) override;
    void Move(glm::vec2 previousMousePosition, glm::vec2 currentMousePosition) override;
    float GetInverseMomentOfInertia() override;

private:
    void Line::CalculateNormal();
};
