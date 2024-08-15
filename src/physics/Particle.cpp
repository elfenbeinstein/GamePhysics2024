#include "Particle.h"

Particle::Particle()
    : Type(None),
      Colour(Colors::white),
      Mass(0),
      Velocity(),
      TotalForce(),
      Position(0, 0),
      InverseMass(),
      Damping(0.0f),
      CoefficientOfRestitution(1.0f),
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
                   float damping,
                   float coefficientOfRestitution, 
                   glm::vec2 velocity,
                   glm::vec2 totalForce,
                   glm::vec2 position,
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
      Position(position),
      InverseMass(),
      Damping(damping),
      CoefficientOfRestitution(coefficientOfRestitution),
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

void Particle::AddImpulseWithTorque(glm::vec2 impulse, glm::vec2 pointOfApplication) {
     Velocity += impulse * InverseMass;
 }

void Particle::AddForce(const glm::vec2& force) {
     TotalForce += force;
 }

float Particle::GetInverseMomentOfInertia() {
     return 1.0f;
 }

float Particle::GetAngularVelocity() {
    return 0.0f;
}

 void Particle::Update(float deltaTime, glm::vec2 gravity) {}
 
 void Particle::OnCollision() {}

 void Particle::Move(glm::vec2 previousMousePosition, glm::vec2 currentMousePosition) {}

 bool Particle::IsMouseOverParticle(glm::vec2 mousePosition) {
     return false;
 }
