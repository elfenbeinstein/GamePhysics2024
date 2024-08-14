#pragma once

#include <vector>
#include "core/Simple2DScene.h"
#include "physics/Circle.h"
#include "physics/AxisAlignedBox.h"

class Assignment5 : public Simple2DScene {
public:
    Assignment5();
    ~Assignment5();

    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void DrawGUI() override;

    virtual const char* GetName() override { return "Assignment5"; };

private:
    float nextCircleRadius;
    glm::vec2 nextCirclePosition;
    glm::vec2 nextBoxBottemLeft;
    glm::vec2 nextBoxTopRight;
    int impulseCircleIndex;
    bool mouseDown;
    glm::vec2 mousePosition;

    const glm::vec2 g = {0, -9.81f};

    std::vector<Circle> circles;
    std::vector<AxisAlignedBox> boxes;

    glm::vec2 Assignment5::GetClosestPoint(AxisAlignedBox& box, Circle& circle);
    int Assignment5::SetCurrentImpulseCircle();
    void Assignment5::AddImpulse(Circle& circle, const glm::vec2& impulse);
    float Assignment5::GetInverseMass(float mass);
};
