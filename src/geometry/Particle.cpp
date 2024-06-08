#include "Particle.h"

Particle::Particle()
    : Type(None),
      Colour(Colors::white),
      Mass(0),
      Velocity(),
      TotalForce(),
      InverseMass(),
      RemoveThisAfterCollision(false),
      RemoveOtherParticleAfterCollision(false),
      CanBeRemovedOnCollision(true),
      CanBeMovedByMouse(false),
      CanAddImpulseByMouse(false) {
    InverseMass = GetInverseMass(Mass);
}

Particle::Particle(ParticleType type,
                   Color colour,
                   float mass,
                   glm::vec2 velocity,
                   glm::vec2 totalForce)
    : Type(type),
      Colour(colour),
      Mass(mass),
      Velocity(velocity),
      TotalForce(totalForce),
      InverseMass(),
      RemoveThisAfterCollision(false),
      RemoveOtherParticleAfterCollision(false),
      CanBeRemovedOnCollision(true),
      CanBeMovedByMouse(false),
      CanAddImpulseByMouse(false) {
    InverseMass = GetInverseMass(Mass);
}

Particle::Particle(ParticleType type,
                   Color colour,
                   float mass,
                   glm::vec2 velocity,
                   glm::vec2 totalForce,
                   bool removeAfterCol,
                   bool removeOtherAfterCol, bool canBeRemoved)
    : Type(type),
      Colour(colour),
      Mass(mass),
      Velocity(velocity),
      TotalForce(totalForce),
      InverseMass(),
      RemoveThisAfterCollision(removeAfterCol),
      RemoveOtherParticleAfterCollision(removeOtherAfterCol),
      CanBeRemovedOnCollision(canBeRemoved),
      CanBeMovedByMouse(false),
      CanAddImpulseByMouse(false) {
    InverseMass = GetInverseMass(Mass);
}

Particle::Particle(ParticleType type,
                   Color colour,
                   float mass,
                   glm::vec2 velocity,
                   glm::vec2 totalForce,
                   bool removeAfterCol,
                   bool removeOtherAfterCol,
                   bool canBeRemoved,
                   bool canBeMoved,
                   bool canAddImpulse)
    : Type(type),
      Colour(colour),
      Mass(mass),
      Velocity(velocity),
      TotalForce(totalForce),
      InverseMass(),
      RemoveThisAfterCollision(removeAfterCol),
      RemoveOtherParticleAfterCollision(removeOtherAfterCol),
      CanBeRemovedOnCollision(canBeRemoved),
      CanBeMovedByMouse(canBeMoved),
      CanAddImpulseByMouse(canAddImpulse) {
    InverseMass = GetInverseMass(Mass);
}

float Particle::GetInverseMass(float mass) {
    if (mass == 0.0f)
        return 0.0f;
    return 1.0f / mass;
}

void Particle::AddImpulse(glm::vec2 impulse) {
     Velocity += impulse * InverseMass;
 }

void Particle::AddForce(const glm::vec2& force) {
     TotalForce += force;
 }

 void Particle::Update(float deltaTime, glm::vec2 gravity) {}

 void Particle::UpdateVelocity(float deltaTime) {
     glm::vec2 acceleration = TotalForce * InverseMass;
     Velocity += acceleration * deltaTime;
 }
 
 void Particle::OnCollision() {}

 void Particle::Move(glm::vec2 previousMousePosition, glm::vec2 currentMousePosition) {}

 bool Particle::IsMouseOverParticle(glm::vec2 mousePosition) {
     return false;
 }
