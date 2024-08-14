#include "CollisionManifold.h"

CollisionManifold::CollisionManifold(glm::vec2 normal, float intersectionDepth)
    : CollisionNormal(normal),
      IntersectionDepth(intersectionDepth),
      ContactPoint1(),
      ContactPoint2(),
      ContactPointCount(0) {}