#include "geometry/RigidBody.h"

RigidBody::RigidBody(ParticleType type,
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
      AngularVelocity(0.0f) {}

RigidBody::RigidBody(ParticleType type,
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
      AngularVelocity(angularVel) {}

void RigidBody::Update(float deltaTime, glm::vec2 gravity) {
    TotalForce = glm::vec2(0);
    AddForce(gravity * Mass);
    glm::vec2 acceleration = TotalForce * InverseMass;
    Velocity += acceleration * deltaTime;
    Position += Velocity * deltaTime;

    Angle += AngularVelocity * deltaTime;
    AngularVelocity += glm::pow(AngularDamp, deltaTime);

    ColorCountdown += deltaTime;
}

void RigidBody::OnCollision() {
    ColorCountdown = -0.3f;
}

void RigidBody::Move(glm::vec2 previousMousePosition,
                  glm::vec2 currentMousePosition) {
    Position += currentMousePosition - previousMousePosition;
}