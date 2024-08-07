#pragma once
#include "core/Simple2DScene.h"
#include "geometry/Particle.h"
#include "core/Colors.h"

class AxisAlignedBox : public Particle {
public:
    AxisAlignedBox();
    AxisAlignedBox(glm::vec2 bottomLeft,
                   glm::vec2 topRight,
                   glm::vec2 vel = {0.0f, 0.0f},
                   glm::vec2 totalForce = {0.0f, 0.0f},
                   float rad = 1.0f,
                   float mass = 1.0f,
                   float damping = 0.0f,
                   float coefficientOfRestitution = 1.0f,
                   bool removeAfterCollision = false,
                   bool removeOtherAfterCollision = false,
                   bool canBeRemoved = true,
                   bool canBeMovedByMouse = false,
                   bool canAddImpulseByMouse = false,
                   Color color = Colors::white);

    glm::vec2 TopRight;

    void Draw() override;

    bool IsMouseOverParticle(glm::vec2 mousePosition) override;
    void Move(glm::vec2 previousMousePosition, glm::vec2 currentMousePosition) override;

    glm::vec2 ClosestPoint(glm::vec2 otherPosition);
};
