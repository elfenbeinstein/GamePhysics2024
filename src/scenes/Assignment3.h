#pragma once

#include <vector>
#include "core/Simple2DScene.h"
#include "geometry/Circle.h"
#include "geometry/Line.h"

class Assignment3 : public Simple2DScene {
public:
    Assignment3();
    ~Assignment3();
    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void DrawGUI() override;

    virtual const char* GetName() override { return "Assignment3"; };

private:
    std::vector<Circle> circles;
    std::vector<Line> lines;

    float nextCircleRadius;
    glm::vec2 nextCirclePosition;
    const glm::vec2 g = {0, -9.81f};

    void Assignment3::CheckForCollision(Circle& circle);
    void Assignment3::CheckForCollisionWithinEndpoints(Circle& circle);
    void Assignment3::ResolveCollision(Circle& circle, Line line, float intersectionDepth);
    void Assignment3::AddForce(Circle& circle, const glm::vec2& force);
    float Assignment3::GetInverseMass(float mass);
};
