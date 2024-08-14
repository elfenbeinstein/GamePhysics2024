#pragma once

#include <vector>
#include "core/Simple2DScene.h"
#include "physics/Circle.h"

class Assignment2 : public Simple2DScene {
public:
    Assignment2();
    ~Assignment2();

    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void DrawGUI() override;

    virtual const char* GetName() override { return "Assignment2"; };

    private:
    float nextCircleRadius;
    glm::vec2 nextCirclePosition;
    float nextCircleMass;

    const glm::vec2 g = {0, -9.81f};

    std::vector<Circle> circles;
    bool showTrajectory;
    std::vector<glm::vec2> trajectory;
    int steps;
    float previewTime;
    int trajectoryMethod;
    int impulseCircleIndex;
    float distanceAfterThrowOnGround;
    float firstCollisionTime;
    glm::vec2 collisionPosition;

    bool mouseDown;
    glm::vec2 mousePosition;

    void Assignment2::AddImpulse(Circle& circle, const glm::vec2& impulse);
    void Assignment2::AddForce(Circle& circle, const glm::vec2& force);
    void Assignment2::DrawTrajectory(float previewTime, glm::vec2 impulse, int steps);
    void Assignment2::DrawTrajectoryWithGroundCollision(float previewTime, glm::vec2 impulse, int steps);
    float Assignment2::GetNextIntersectionTime(float previewTime, Circle& circle, glm::vec2 impulse, float inverseMass);
    float Assignment2::DistanceFromOriginalPositionWhenCircleHitsGround(glm::vec2 startPosition, glm::vec2 startVelocity, glm::vec2 acceleration, float collisionTime);

    int Assignment2::SetCurrentImpulseCircle();
    float Assignment2::GetInverseMass(float mass);
};
