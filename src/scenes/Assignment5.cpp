#include "Assignment5.h"

#include "imgui.h"
#include "core/Draw.h"
#include "core/Colors.h"

Assignment5::Assignment5()
    : nextCirclePosition(0, 3),
      nextCircleRadius(0.5),
      nextBoxBottemLeft(0.0f, 0.0f),
      nextBoxTopRight(1.0f, 1.0f),
      circles(),
      boxes(),
      impulseCircleIndex(-1),
      mouseDown(false),
      mousePosition() {
    boxes.push_back(AxisAlignedBox::AxisAlignedBox({-1.0f, 1.0f}, {1.0f, 2.0f}));
    boxes.push_back(AxisAlignedBox::AxisAlignedBox({-3.0f, 2.0f}, {-2.0f, 3.0f}));
}

Assignment5::~Assignment5() {}

void Assignment5::OnEnable() {}

void Assignment5::OnDisable() {}

void Assignment5::Update(float deltaTime) {
    mousePosition = Input::GetMousePos();
    if (Input::IsMouseClicked(0)) {
        impulseCircleIndex = SetCurrentImpulseCircle();
        if (impulseCircleIndex != -1)
            mouseDown = true;
    }
    if (Input::IsMouseReleased(0)) {
        if (mouseDown && impulseCircleIndex != -1 &&
            impulseCircleIndex < circles.size()) {
            AddImpulse(circles[impulseCircleIndex],
                       mousePosition - circles[impulseCircleIndex].Position);
        }
        mouseDown = false;
        impulseCircleIndex = -1;
    }


    for (std::size_t i = 0; i < circles.size(); ++i) {
        if (i == impulseCircleIndex)
            continue;
        auto& circle = circles[i];

        circle.Velocity = circle.Velocity + g * deltaTime;
        circle.Position = circle.Position + circle.Velocity * deltaTime;

        for (int i = boxes.size() - 1; i >= 0; --i) {
            glm::vec2 closestPoint = GetClosestPoint(boxes[i], circle);
            float distanceSquared = std::pow(circle.Position.x - closestPoint.x, 2) +
                  std::pow(circle.Position.y - closestPoint.y, 2);
            if (distanceSquared > std::pow(circle.Radius, 2))
                continue;

            float intersectionDepth = circle.Radius - std::sqrt(distanceSquared);
            glm::vec2 normal = glm::normalize(circle.Position - closestPoint);
            
            circle.Position += normal * intersectionDepth;
            circle.Velocity = circle.Velocity -
                              2.0f * glm::dot(circle.Velocity, normal) * normal;

            std::vector<AxisAlignedBox>::iterator boxToRemove = boxes.begin() + i;
            boxes.erase(boxToRemove);
        }

        if (circle.Position.y <= circle.Radius) {
            circle.Position.y = circle.Radius;
            circle.Velocity.y *= -1.0f;
        }
    }
}

void Assignment5::Draw() {
    for (const auto& circle : circles) {
        Draw::Circle(circle.Position, circle.Radius);
    }

    for (const auto& box : boxes) {
        Draw::AABB(box.BottomLeft, box.TopRight);
    }

    if (mouseDown && impulseCircleIndex != -1 &&
        impulseCircleIndex < circles.size()) {
        Draw::SetColor(Colors::red);
        Draw::Arrow(circles[impulseCircleIndex].Position, mousePosition);
        Draw::SetColor(Colors::white);
    }

    Draw::Line({-20.0f, 0.0f}, {20.0f, 0.0f});
}

void Assignment5::DrawGUI() {
    ImGui::Begin("Inspector");
    ImGui::DragFloat2("Next Position", &nextCirclePosition[0], 0.1f);
    ImGui::DragFloat("Next Radius", &nextCircleRadius, 0.1f);
    if (ImGui::Button("Add new Circle")) {
        circles.push_back(Circle(nextCirclePosition, {0, 0}, nextCircleRadius));
    }
    if (ImGui::Button("Remove all Circles")) {
        circles.clear();
    }
    ImGui::Spacing();
    ImGui::DragFloat2("BottomLeft", &nextBoxBottemLeft[0], 0.1f);
    ImGui::DragFloat2("TopRight", &nextBoxTopRight[0], 0.1f);
    if (ImGui::Button("Add new Box")) {
        boxes.push_back(AxisAlignedBox(nextBoxBottemLeft, nextBoxTopRight));
    }
    if (ImGui::Button("Remove all Boxes")) {
        boxes.clear();
    }
    ImGui::End();
}

glm::vec2 Assignment5::GetClosestPoint(AxisAlignedBox& box, Circle& circle) {
    float x = glm::clamp(circle.Position.x, box.BottomLeft.x, box.TopRight.x);
    float y = glm::clamp(circle.Position.y, box.BottomLeft.y, box.TopRight.y);
    return {x, y};
}

int Assignment5::SetCurrentImpulseCircle() {
    for (std::size_t i = 0; i < circles.size(); ++i) {
        float distance = glm::distance(mousePosition, circles[i].Position);
        if (distance <= circles[i].Radius)
            return i;
    }
    return -1;
}

void Assignment5::AddImpulse(Circle& circle, const glm::vec2& impulse) {
    circle.Velocity += impulse * GetInverseMass(circle.Mass);
}

float Assignment5::GetInverseMass(float mass) {
    if (mass == 0.0f)
        return 0.0f;
    return 1.0f / mass;
}
