#include "AxisAlignedBox.h"
#include "core/Colors.h"
#include "core/Draw.h"

AxisAlignedBox::AxisAlignedBox()
    : Particle(Particle::AABB, Colors::white, 0.0f, 0.0f, 1.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, true, false, true, false, false),
      TopRight(1.0f, 1.0f) {}

AxisAlignedBox::AxisAlignedBox(glm::vec2 bottomLeft,
                               glm::vec2 topRight,
                               glm::vec2 vel,
                               glm::vec2 totalForce,
                               float rad,
                               float mass,
                               float damping,
                               float coefficientOfRestitution,
                               bool removeAfterCollision,
                               bool removeOtherAfterCollision,
                               bool canBeRemoved,
                               bool canBeMovedByMouse,
                               bool canAddImpulseByMouse,
                               Color color)
    : Particle(Particle::AABB,
               color,
               mass,
               damping,
               coefficientOfRestitution,
               vel,
               totalForce,
               bottomLeft,
               removeAfterCollision,
               removeOtherAfterCollision,
               canBeRemoved,
               canBeMovedByMouse,
               canAddImpulseByMouse),
      TopRight(topRight) {}

void AxisAlignedBox::Draw() {
    Draw::SetColor(Colour);
    Draw::AABB(Position, TopRight);
    Draw::SetColor(Colors::white);
}

bool AxisAlignedBox::IsMouseOverParticle(glm::vec2 mousePosition) {
    glm::vec2 closestPoint = ClosestPoint(mousePosition);
    float distanceSquared = std::pow(mousePosition.x - closestPoint.x, 2) +
                            std::pow(mousePosition.y - closestPoint.y, 2);
    return distanceSquared <= 0.0f;
}

void AxisAlignedBox::Move(glm::vec2 previousMousePosition, glm::vec2 currentMousePosition) {
    Position += currentMousePosition - previousMousePosition;
    TopRight += currentMousePosition - previousMousePosition;
}

glm::vec2 AxisAlignedBox::ClosestPoint(glm::vec2 otherPosition) {
    float x = glm::clamp(otherPosition.x, Position.x, TopRight.x);
    float y = glm::clamp(otherPosition.y, Position.y, TopRight.y);
    return {x, y};
}