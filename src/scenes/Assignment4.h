#pragma once

#include <vector>
#include "core/Simple2DScene.h"
#include "physics/Circle.h"
#include "physics/Line.h"
#include <random>

class Assignment4 : public Simple2DScene {
public:
    Assignment4();
    ~Assignment4();

    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void DrawGUI() override;

    virtual const char* GetName() override { return "Assignment4"; };

private:
    std::vector<Circle> circles;
    std::vector<Line> lines;

    std::mt19937 gen;
    const glm::vec2 g = {0, -9.81f};
    bool takeMassIntoAccount;

    Circle Assignment4::CreateRandomCircle(bool isStatic);
    void Assignment4::AddForce(Circle& circle, const glm::vec2& force);
    float Assignment4::GetInverseMass(float mass);
    void Assignment4::CheckForCollisionWithLines(Circle& circle);
    void Assignment4::ResolveCollision(Circle& circle, Line line, float intersectionDepth);
    void Assignment4::CheckForCollisionBetweenCircles(Circle& circle1, Circle& circle2);
    void Assignment4::CheckForCollisionBetweenCirclesTakingMassIntoAccount(Circle& circle1, Circle& circle2);
};
