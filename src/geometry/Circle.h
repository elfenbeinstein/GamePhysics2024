#pragma once
#include "core/Simple2DScene.h"
#include "geometry/Particle.h"
#include "geometry/RigidBody.h"

class Circle : public RigidBody {
public:
    Circle();
    Circle(glm::vec2 pos,
           glm::vec2 vel = {0.0f, 0.0f},
           float rad = 1.0f,
           float mass = 1.0f,
           float damping = 0.0f,
           float coefficientOfRestitution = 1.0f,
           bool removeAfterCollision = false,
           bool removeOtherAfterCollision = false,
           bool canBeRemoved = true,
           bool canBeMovedByMouse = false,
           bool canAddImpulseByMouse = false,
           Color colour = Colors::white,
           float angle = 0.0f,
           float angularDamp = 0.0f,
           float angularVel = 0.0f);
    
    glm::vec2 Acceleration;
    float Radius;

    void Draw() override;
    bool IsMouseOverParticle(glm::vec2 mousePosition) override;
};
