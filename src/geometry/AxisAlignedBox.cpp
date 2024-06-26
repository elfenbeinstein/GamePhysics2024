#include "AxisAlignedBox.h"
#include "core/Colors.h"
#include "core/Draw.h"

AxisAlignedBox::AxisAlignedBox()
    : Particle(Particle::AABB, Colors::white, 0.0f, 0.0f, 1.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, true, false, true, false, false),
      TopRightOffset(1.0f, 1.0f) {}

AxisAlignedBox::AxisAlignedBox(glm::vec2 bottomLeft, glm::vec2 topRight)
    : Particle(Particle::AABB, Colors::white, 0.0f, 0.0f, 1.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, bottomLeft, true, false, true, false, false),
      TopRightOffset(topRight) {}

AxisAlignedBox::AxisAlignedBox(glm::vec2 bottomLeft, glm::vec2 topRight, bool removeAfterCol, bool moveByMouseDrag)
    : Particle(Particle::AABB, Colors::white, 0.0f, 0.0f, 1.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, bottomLeft, removeAfterCol, false, true, moveByMouseDrag, false),
      TopRightOffset(topRight) {}

AxisAlignedBox::AxisAlignedBox(glm::vec2 bottomLeft, glm::vec2 topRight, bool removeAfterCol, bool removeOtherAfterCol, bool canBeRemoved, bool moveByMouseDrag, bool canAddImpulse)
    : Particle(Particle::AABB, Colors::white, 0.0f, 0.0f, 1.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, bottomLeft, removeAfterCol, removeOtherAfterCol, canBeRemoved, moveByMouseDrag, canAddImpulse),
      TopRightOffset(topRight) {}

void AxisAlignedBox::Draw() {
    Draw::SetColor(Colour);
    Draw::AABB(Position, TopRightOffset);
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
    TopRightOffset += currentMousePosition - previousMousePosition;
}

glm::vec2 AxisAlignedBox::ClosestPoint(glm::vec2 otherPosition) {
    float x = glm::clamp(otherPosition.x, Position.x, TopRightOffset.x);
    float y = glm::clamp(otherPosition.y, Position.y, TopRightOffset.y);
    return {x, y};
}