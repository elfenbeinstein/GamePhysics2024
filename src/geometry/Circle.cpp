#include "Circle.h"
#include "core/Colors.h"
#include "core/Simple2DScene.h"
#include "core/Draw.h"

Circle::Circle()
    : RigidBody(Particle::Circle, Colors::white, 1.0f, 0.0f, 1.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, false, false, true, false, false),
      Acceleration(0, 0),
      Radius(0.5f) {}

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
               bool canAddImpulseByMouse,
               Color colour,
               float angle,
               float angularDamp,
               float angularVel)
    : RigidBody(Particle::Circle, colour, mass, damping, coefficientOfRestitution, angle, angularVel, angularDamp, vel, {0.0f, 0.0f}, pos, removeAfterCollision, removeOtherAfterCollision, canBeRemoved, canBeMovedByMouse, canAddImpulseByMouse),
      Acceleration(0, 0),
      Radius(rad) {}


void Circle::Draw() {
    Color col = Colour;
    if (ColorCountdown < 0.0f)
        col = Colors::red;
    Draw::SetColor(col);
    Draw::Circle(Position, Radius);
    Draw::SetColor(Colors::white);
}

bool Circle::IsMouseOverParticle(glm::vec2 mousePosition) {
    float distanceSquared = std::pow(mousePosition.x - Position.x, 2) +
                            std::pow(mousePosition.y - Position.y, 2);
    return std::pow(Radius, 2) > distanceSquared;
}