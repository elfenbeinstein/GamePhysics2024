#include "Circle.h"
#include "core/Colors.h"
#include "core/Simple2DScene.h"
#include "core/Draw.h"

Circle::Circle()
    : Particle(Particle::Circle, Colors::white, 1.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}),
      Acceleration(0, 0),
      Radius(0.5f),
      ColorCountdown(0.0f) {}

Circle::Circle(glm::vec2 pos, glm::vec2 vel, float rad)
    : Particle(Particle::Circle, Colors::white, 1.0f, vel, {0.0f, 0.0f}, pos),
      Acceleration(0, 0),
      Radius(rad),
      ColorCountdown(0.0f) {}

Circle::Circle(glm::vec2 pos, glm::vec2 vel, float rad, float mass) 
    : Particle(Particle::Circle, Colors::white, mass, vel, {0.0f, 0.0f}, pos),
      Acceleration(0, 0),
      Radius(rad),
      ColorCountdown(0.0f) {}

Circle::Circle(glm::vec2 pos,
    glm::vec2 vel,
    float rad,
    float mass,
    bool removeAfterCollision,
    bool removeOtherAfterCollision,
    bool canBeRemoved,
    bool canBeMovedByMouse,
    bool canAddImpulseByMouse)
    : Particle(Particle::Circle, Colors::white, mass, vel, {0.0f, 0.0f}, pos, removeAfterCollision, removeOtherAfterCollision, canBeRemoved, canBeMovedByMouse, canAddImpulseByMouse), 
      Acceleration(0, 0),
      Radius(rad),
      ColorCountdown(0.0f) {}

Circle::Circle(glm::vec2 pos,
               glm::vec2 vel,
               float rad,
               float mass,
               float damping,
               float coefficientOfRestitution,
               bool removeAfterCollision,
               bool removeOtherAfterCollision,
               bool canBeRemoved,
               bool canBeMovedByMouse,
               bool canAddImpulseByMouse)
    : Particle(Particle::Circle, Colors::white, mass, damping, coefficientOfRestitution, vel, {0.0f, 0.0f}, pos, removeAfterCollision, removeOtherAfterCollision, canBeRemoved, canBeMovedByMouse, canAddImpulseByMouse),
      Acceleration(0, 0),
      Radius(rad),
      ColorCountdown(0.0f) {}

void Circle::Draw() {
    Draw::SetColor(Colour);
    Draw::Circle(Position, Radius);
    Draw::SetColor(Colors::white);
}

void Circle::Update(float deltaTime, glm::vec2 gravity) {
    TotalForce = glm::vec2(0);
    AddForce(gravity * Mass);
    UpdateVelocity(deltaTime);
    Position += Velocity * deltaTime;

    if (Colour == Colors::red) {
        ColorCountdown += deltaTime;
        if (ColorCountdown > 0.3f)
            Colour = Colors::white;
    }
}

void Circle::OnCollision() {
    Colour = Colors::red;
    ColorCountdown = 0.0f;
}

void Circle::Move(glm::vec2 previousMousePosition, glm::vec2 currentMousePosition) {
    Position += currentMousePosition - previousMousePosition;
}

bool Circle::IsMouseOverParticle(glm::vec2 mousePosition) {
    float distanceSquared = std::pow(mousePosition.x - Position.x, 2) +
                            std::pow(mousePosition.y - Position.y, 2);
    return std::pow(Radius, 2) > distanceSquared;
}