#pragma once
#include "core/Simple2DScene.h"

class Line {
public:
    Line();
    Line(glm::vec2 start, glm::vec2 end);

    glm::vec2 Start;
    glm::vec2 End;
};
