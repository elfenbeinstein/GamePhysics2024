#pragma once
#include "core/Simple2DScene.h"
#include "geometry/Particle.h"

class RigidBody : public Particle {
public:
    RigidBody(ParticleType type,
              Color color,
              float mass,
              float damping,
              float coefficientOfRestitution,
              glm::vec2 vel,
              glm::vec2 totalForce,
              glm::vec2 pos,
              bool removeAfterCollision,
              bool removeOtherAfterCollision,
              bool canBeRemoved,
              bool canBeMovedByMouse,
              bool canAddImpulseByMouse);
    RigidBody(ParticleType type,
              Color color,
              float mass,
              float damping,
              float coefficientOfRestitution,
              float angle,
              float angularDamp,
              float angularVel,
              glm::vec2 vel,
              glm::vec2 totalForce,
              glm::vec2 pos,
              bool removeAfterCollision,
              bool removeOtherAfterCollision,
              bool canBeRemoved,
              bool canBeMovedByMouse,
              bool canAddImpulseByMouse);

    float ColorCountdown;
    float Angle;
    float AngularDamp;
    float AngularVelocity;
    void Update(float deltaTime, glm::vec2 gravity) override;
    void OnCollision() override;
    void Move(glm::vec2 previousMousePosition, glm::vec2 currentMousePosition) override;

};