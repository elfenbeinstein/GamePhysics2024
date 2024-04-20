#pragma once

#include <vector>
#include "core/Simple2DScene.h"
#include "geometry/Circle.h"
#include "geometry/Line.h"
#include <random>

class Assignment1 : public Simple2DScene {
public:
    Assignment1();
    ~Assignment1();

    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void DrawGUI() override;

    virtual const char* GetName() override { return "Assignment1"; };

    private:
    float nextCircleRadius;
    glm::vec2 nextCirclePosition;
    glm::vec2 nextCircleVelocity;
    const glm::vec2 g = {0, -9.81f};

    std::vector<Circle> circles;
    std::vector<Line> verticalLines;
    std::vector<Line> horizontalLines;

    std::mt19937 gen;

    Circle Assignment1::CreateRandomCircle();
    void Assignment1::CheckForCollisions(Circle& circle);
    bool Assignment1::CollidedWithVerticalLine(Circle& circle);
    bool Assignment1::CollidedWithHorizontalLine(Circle& circle);
    float Assignment1::VectorDistance(glm::vec2 v1, glm::vec2 v2);

    bool Assignment1::CollidesWithInfinite(float circle, float line, float radius);
    bool Assignment1::IsBetweenEndpoints(float circle, float start, float end);
    bool Assignment1::CollidesWithEndpoint(glm::vec2 circle, glm::vec2 endpoint, float radius);
};
