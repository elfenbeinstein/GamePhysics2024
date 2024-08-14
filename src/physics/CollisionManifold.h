#pragma once

#include <memory>
#include <vector>
#include "core/Simple2DScene.h"

class CollisionManifold {
public:
    CollisionManifold(glm::vec2 normal, float intersectionDepth);

    glm::vec2 CollisionNormal;
    float IntersectionDepth;
    glm::vec2 ContactPoint1;
    glm::vec2 ContactPoint2;
    int ContactPointCount;
};
