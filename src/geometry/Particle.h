#pragma once
#include "core/Simple2DScene.h"
#include "core/Colors.h"

class Particle {
public:
    enum ParticleType { None, Circle, Line, AABB };

    Particle::Particle();
    Particle::Particle(ParticleType type, Color colour, float mass, glm::vec2 velocity, glm::vec2 totalForce, glm::vec2 position);
    Particle::Particle(ParticleType type, Color colour, float mass, glm::vec2 velocity, glm::vec2 totalForce, glm::vec2 position, bool removeAfterCol, bool removeOtherAfterCol, bool canBeRemoved);
    Particle::Particle(ParticleType type, Color colour, float mass, glm::vec2 velocity, glm::vec2 totalForce, glm::vec2 position, bool removeAfterCol, bool removeOtherAfterCol, bool canBeRemoved, bool canBeMoved, bool canAddImpulse);
    Particle::Particle(ParticleType type, Color colour, float mass, float damping, float coefficientOfRestitution, glm::vec2 velocity, glm::vec2 totalForce, glm::vec2 position, bool removeAfterCol, bool removeOtherAfterCol, bool canBeRemoved, bool canBeMoved, bool canAddImpulse);

    float GetInverseMass(float mass);
    void AddImpulse(glm::vec2 impulse);
    void Particle::AddForce(const glm::vec2& force);

    virtual void Draw() = 0;
    virtual void Update(float deltaTime, glm::vec2 gravity);
    virtual void UpdateVelocity(float deltaTime);
    virtual void OnCollision();
    virtual void Move(glm::vec2 previousMousePosition, glm::vec2 currentMousePosition);

    virtual bool Particle::IsMouseOverParticle(glm::vec2 mousePosition);

    ParticleType Type;
    Color Colour;
    float Mass;
    float InverseMass;
    float Damping;
    float CoefficientOfRestitution;
    glm::vec2 Velocity;
    glm::vec2 TotalForce;
    glm::vec2 Position;

    bool RemoveThisAfterCollision;
    bool RemoveOtherParticleAfterCollision;
    bool CanBeRemovedOnCollision;

    bool CanBeMovedByMouse;
    bool CanAddImpulseByMouse;
};