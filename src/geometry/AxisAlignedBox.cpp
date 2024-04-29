#include "AxisAlignedBox.h"

AxisAlignedBox::AxisAlignedBox() : BottomLeft(0.0f, 0.0f), TopRight(1.0f, 1.0f) {}

AxisAlignedBox::AxisAlignedBox(glm::vec2 bottomLeft, glm::vec2 topRight)
    : BottomLeft(bottomLeft), TopRight(topRight) {}