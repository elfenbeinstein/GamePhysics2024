#pragma once
#include "core/Simple2DScene.h"

class AxisAlignedBox {
public:
    AxisAlignedBox();
    AxisAlignedBox(glm::vec2 bottomLeft, glm::vec2 topRight);

    glm::vec2 BottomLeft;
    glm::vec2 TopRight;
};
