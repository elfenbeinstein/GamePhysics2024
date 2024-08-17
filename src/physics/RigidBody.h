#pragma once
#include "core/Simple2DScene.h"
#include "core/PhysicsUtils.h"
#include "physics/Particle.h"

class RigidBody : public Particle {
public:
    RigidBody(ParticleType type,
              Color color,
              float mass,
              float damping,
              float coefficientOfRestitution,
              float momentOfInertia,
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
              float momentOfInertia,
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
    float TotalTorque;
    float MomentOfInertia;
    float InverseMomentOfInertia;
    float MagneticFieldStrength;
    void Update(float deltaTime, glm::vec2 gravity) override;
    void OnCollision() override;
    void Move(glm::vec2 previousMousePosition, glm::vec2 currentMousePosition) override;
    void AddImpulseWithTorque(glm::vec2 impulse, glm::vec2 pointOfApplication) override;
    void AddTorque(float torque);

    float GetInverseMomentOfInertia() override;
    float GetAngularVelocity() override;
};