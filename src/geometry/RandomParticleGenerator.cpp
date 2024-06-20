#include "RandomParticleGenerator.h"

RandomParticleGenerator::RandomParticleGenerator() {
    std::random_device rd;
    gen = std::mt19937(rd());
}

Circle RandomParticleGenerator::CreateRandomCircle(
    float minRadius,
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
    bool canAddImpulseByMouse) {
    float radius =
        std::uniform_real_distribution<float>(minRadius, maxRadius)(gen);

    std::uniform_real_distribution<float> pos(minPosition, maxPosition);
    float xPos = pos(gen);
    float yPos = pos(gen);
    glm::vec2 position(xPos, yPos);

    std::uniform_real_distribution<float> vel(minVelocity, maxVelocity);
    float xVel = vel(gen);
    float yVel = vel(gen);
    glm::vec2 velocity(xVel, yVel);

    float mass = std::uniform_real_distribution<float>(minMass, maxMass)(gen);
    float damp = std::uniform_real_distribution<float>(minDamp, maxDamp)(gen);
    float coefficientOfRestitution = std::uniform_real_distribution<float>(
        minCoefficient, maxCoefficient)(gen);

    return Circle::Circle(position, velocity, radius, mass, damp, coefficientOfRestitution, removeAfterCollision, removeOtherAfterCollision, canBeRemoved, canBeMovedByMouse, canAddImpulseByMouse);
}