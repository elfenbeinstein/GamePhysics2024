#pragma once
#include "core/Simple2DScene.h"
#include "core/Math.h"
#include "geometry/Particle.h"
#include "geometry/RigidBody.h"

class Rectangle : public RigidBody {
public:
    Rectangle(glm::vec2 halfExtends,
              glm::vec2 pos,
              glm::vec2 vel,
              float mass,
              float damping,
              float coefficientOfRestitution,
              float angle,
              float angularVelocity,
              float angularDamp,
              bool removeAfterCollision,
              bool removeOtherAfterCollision,
              bool canBeRemoved,
              bool canBeMovedByMouse,
              bool canAddImpulseByMouse,
              Color colour);

    void Draw() override;
    bool IsMouseOverParticle(glm::vec2 mousePosition) override;
    glm::vec2 HalfExtends;
};
