#include "AxisAlignedBox.h"
#include "core/Colors.h"
#include "core/Draw.h"

AxisAlignedBox::AxisAlignedBox()
    : Particle(Particle::AABB, Colors::white, 0.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, true, false, true, false, false),
      BottomLeft(0.0f, 0.0f),
      TopRight(1.0f, 1.0f) {}

AxisAlignedBox::AxisAlignedBox(glm::vec2 bottomLeft, glm::vec2 topRight)
    : Particle(Particle::AABB, Colors::white, 0.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, true, false, true, false, false),
      BottomLeft(bottomLeft),
      TopRight(topRight) {}

AxisAlignedBox::AxisAlignedBox(glm::vec2 bottomLeft, glm::vec2 topRight, bool removeAfterCol, bool moveByMouseDrag)
    : Particle(Particle::AABB, Colors::white, 0.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, removeAfterCol, false, true, moveByMouseDrag, false),
      BottomLeft(bottomLeft),
      TopRight(topRight) {}

AxisAlignedBox::AxisAlignedBox(glm::vec2 bottomLeft, glm::vec2 topRight, bool removeAfterCol, bool removeOtherAfterCol, bool canBeRemoved, bool moveByMouseDrag, bool canAddImpulse)
    : Particle(Particle::AABB, Colors::white, 0.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, removeAfterCol, removeOtherAfterCol, canBeRemoved, moveByMouseDrag, canAddImpulse),
      BottomLeft(bottomLeft),
      TopRight(topRight){}

void AxisAlignedBox::Draw() {
    Draw::SetColor(Colour);
    Draw::AABB(BottomLeft, TopRight);
    Draw::SetColor(Colors::white);
}

bool AxisAlignedBox::IsMouseOverParticle(glm::vec2 mousePosition) {
    glm::vec2 closestPoint = ClosestPoint(mousePosition);
    float distanceSquared = std::pow(mousePosition.x - closestPoint.x, 2) +
                            std::pow(mousePosition.y - closestPoint.y, 2);
    return distanceSquared <= 0.0f;
}

void AxisAlignedBox::Move(glm::vec2 previousMousePosition, glm::vec2 currentMousePosition) {
    BottomLeft += currentMousePosition - previousMousePosition;
    TopRight += currentMousePosition - previousMousePosition;
}

glm::vec2 AxisAlignedBox::ClosestPoint(glm::vec2 otherPosition) {
    float x = glm::clamp(otherPosition.x, BottomLeft.x, TopRight.x);
    float y = glm::clamp(otherPosition.y, BottomLeft.y, TopRight.y);
    return {x, y};
}