#pragma once

#include "geometry/Circle.h"
#include "geometry/Line.h"
#include "geometry/AxisAlignedBox.h"
#include <memory>
#include <vector>
#include <functional>
#include <random>

using CollisionCallback = std::function<void(std::shared_ptr<Particle>&, std::shared_ptr<Particle>&)>;

class RandomParticleGenerator {
public:
    RandomParticleGenerator();
    Circle CreateRandomCircle(float minRadius,
                              float maxRadius,
                              float minPosition,
                              float maxPosition,
                              float minVelocity,
                              float maxVelocity,
                              float minMass,
                              float maxMass,
                              float minDamp,
                              float maxDamp, 
                              float minCoefficient, 
                              float maxCoefficient,
                              bool removeAfterCollision,
                              bool removeOtherAfterCollision,
                              bool canBeRemoved,
                              bool canBeMovedByMouse,
                              bool canAddImpulseByMouse);

private:
    std::mt19937 gen;
};
