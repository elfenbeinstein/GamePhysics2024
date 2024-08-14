#include "physics/RigidBody.h"

RigidBody::RigidBody(ParticleType type,
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
                     bool canAddImpulseByMouse)
    : Particle(type,
               color,
               mass,
               damping,
               coefficientOfRestitution,
               vel,
               totalForce,
               pos,
               removeAfterCollision,
               removeOtherAfterCollision,
               canBeRemoved,
               canBeMovedByMouse,
               canAddImpulseByMouse),
      ColorCountdown(0.0f),
      Angle(0.0f),
      AngularDamp(0.0f),
      AngularVelocity(0.0f),
      MomentOfInertia(momentOfInertia),
      InverseMomentOfInertia(0.0f),
      TotalTorque(0.0f) {
    if (MomentOfInertia != 0.0f)
        InverseMomentOfInertia = 1.0f / MomentOfInertia;
}

RigidBody::RigidBody(ParticleType type,
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
                     bool canAddImpulseByMouse)
    : Particle(type,
               color,
               mass,
               damping,
               coefficientOfRestitution,
               vel,
               totalForce,
               pos,
               removeAfterCollision,
               removeOtherAfterCollision,
               canBeRemoved,
               canBeMovedByMouse,
               canAddImpulseByMouse),
      ColorCountdown(0.0f),
      Angle(angle),
      AngularDamp(angularDamp),
      AngularVelocity(angularVel),
      MomentOfInertia(momentOfInertia),
      InverseMomentOfInertia(0.0f),
      TotalTorque(0.0f) {
    if (MomentOfInertia != 0.0f)
        InverseMomentOfInertia = 1.0f / MomentOfInertia;
}

void RigidBody::Update(float deltaTime, glm::vec2 gravity) {
    TotalForce = glm::vec2(0);
    AddForce(gravity * Mass);
    glm::vec2 acceleration = TotalForce * InverseMass;
    Velocity += acceleration * deltaTime;
    Position += Velocity * deltaTime;

    float angularAcceleration = TotalTorque * MomentOfInertia;
    AngularVelocity += angularAcceleration * deltaTime;
    Angle += AngularVelocity * deltaTime;
    AngularVelocity *= 1.0f - AngularDamp * deltaTime;

    ColorCountdown += deltaTime;
}

void RigidBody::OnCollision() {
    ColorCountdown = -0.3f;
}

void RigidBody::Move(glm::vec2 previousMousePosition,
                  glm::vec2 currentMousePosition) {
    Position += currentMousePosition - previousMousePosition;
}

void RigidBody::AddImpulseWithTorque(glm::vec2 impulse,
                                     glm::vec2 pointOfApplication) {
    Particle::AddImpulse(impulse);
    glm::vec2 r = pointOfApplication - Position;
    float impulsiveTorque = Math::CrossProduct2D(r, impulse);
    AngularVelocity += impulsiveTorque * InverseMomentOfInertia;
}

void RigidBody::AddTorque(float torque) {
    TotalTorque += torque;
}