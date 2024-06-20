#pragma once
#include "core/Simple2DScene.h"
#include "geometry/Particle.h"
#include "core/Colors.h"

class AxisAlignedBox : public Particle {
public:
    AxisAlignedBox();
    AxisAlignedBox(glm::vec2 bottomLeft, glm::vec2 topRight);
    AxisAlignedBox(glm::vec2 bottomLeft,
                   glm::vec2 topRight,
                   bool removeAfterCol,
                   bool moveByMouseDrag);
    AxisAlignedBox::AxisAlignedBox(glm::vec2 bottomLeft,
                                   glm::vec2 topRight,
                                   bool removeAfterCol,
                                   bool removeOtherAfterCol,
                                   bool canBeRemoved,
                                   bool moveByMouseDrag,
                                   bool canAddImpulse);

    glm::vec2 TopRightOffset;

    void Draw() override;

    bool IsMouseOverParticle(glm::vec2 mousePosition) override;
    void Move(glm::vec2 previousMousePosition, glm::vec2 currentMousePosition) override;

    glm::vec2 ClosestPoint(glm::vec2 otherPosition);
};
