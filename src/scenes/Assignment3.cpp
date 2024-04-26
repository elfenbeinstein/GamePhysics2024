#include "Assignment3.h"

#include "imgui.h"
#include <vector>
#include "core/Simple2DScene.h"
#include "geometry/Circle.h"
#include "geometry/Line.h"
#include "core/Draw.h"

Assignment3::Assignment3()
    : circles(), lines(), nextCirclePosition(0, 3), nextCircleRadius(0.5) {
    lines.push_back(Line::Line({-3.0f, 2.0f}, {15.0f, -2.0f}));
    lines.push_back(Line::Line({-30.0f, -2.0f}, {-5.0f, 0.0f}));
    //lines.push_back(Line::Line({0.0f, 2.0f}, {0.0f, -2.0f}));
}

Assignment3::~Assignment3() {}

void Assignment3::OnEnable() {}

void Assignment3::OnDisable() {}

void Assignment3::Update(float deltaTime) {
    for (auto& circle : circles) {
        circle.TotalForce = glm::vec2(0);
        AddForce(circle, g);
        circle.Acceleration = circle.TotalForce * GetInverseMass(circle.Mass);
        circle.Velocity += circle.Acceleration * deltaTime;
        circle.Position += circle.Velocity * deltaTime;

        //CheckForCollision(circle);
        CheckForCollisionWithinEndpoints(circle);
    }
}

void Assignment3::Draw() {
    Draw::Circle(glm::vec2{0.0f, 0.0f}, 0.05f);
    for (const auto& circle : circles) {
        Draw::Circle(circle.Position, circle.Radius);
    }

    for (const auto& line : lines) {
        Draw::Line(line.Start, line.End);
    }
}

void Assignment3::DrawGUI() {
    ImGui::Begin("Inspector");
    ImGui::Text("Next Circle Values:");
    ImGui::DragFloat2("Position", &nextCirclePosition[0], 0.1f, -10.0f, 10.0f);
    ImGui::DragFloat("Radius", &nextCircleRadius, 0.1f, 0.01f, 5.0f);
    if (ImGui::Button("Add New Circle")) {
        circles.push_back(
            Circle(nextCirclePosition, glm::vec2{0.0f, 0.0f}, nextCircleRadius));
    }
    if (ImGui::Button("Remove all circles"))
        circles.clear();
    ImGui::End();
}

void Assignment3::AddForce(Circle& circle, const glm::vec2& force) {
    circle.TotalForce += force * circle.Mass;
}

float Assignment3::GetInverseMass(float mass) {
    if (mass == 0.0f)
        return 0.0f;
    return 1.0f / mass;
}

// checks for collision without taking line ends into account
void Assignment3::CheckForCollision(Circle& circle) {
    for (const auto& line : lines) {
        float projectedDistance = glm::dot(circle.Position, line.Normal) - line.ProjectionOntoNormal;
        if (projectedDistance <= circle.Radius) {
            float intersectionDepth = circle.Radius - projectedDistance;
            circle.Position += intersectionDepth * line.Normal;
            circle.Velocity -= 2.0f * glm::dot(circle.Velocity, line.Normal) * line.Normal;
        }
    }
}

// this function now correctly checks if it collides with the line at fixed end points
// wonky behaviour remains if the circle collides directly with a line from above that is orthogonal to x axis
// it will slide down the side of it, always on side of normal regardless of where circle was before
// fixable but beyond scope of assignment + also very rare case
void Assignment3::CheckForCollisionWithinEndpoints(Circle& circle) {
    for (const auto& line : lines) {
        float radiusSquared = std::pow(circle.Radius, 2);
        float intersectionDepth = 0.0f;

        float distanceToStartSquared =
            std::pow(line.Start.x - circle.Position.x, 2) + 
            std::pow(line.Start.y - circle.Position.y, 2);
        if (distanceToStartSquared <= radiusSquared) {
            intersectionDepth = std::sqrt(radiusSquared - distanceToStartSquared);
            ResolveCollision(circle, line, intersectionDepth);
            continue;
        }

        float distanceToEndSquared =
            std::pow(line.End.x - circle.Position.x, 2) +
            std::pow(line.End.y - circle.Position.y, 2);
        if (distanceToEndSquared <= radiusSquared) {
            intersectionDepth = std::sqrt(radiusSquared - distanceToEndSquared);
            ResolveCollision(circle, line, intersectionDepth);
            continue;
        } 

        float projectedDistance =
            glm::dot(circle.Position, line.Normal) - line.ProjectionOntoNormal;
        
        // need to check if projected distance is positive --> if negative it's on other side of the line
        if (projectedDistance > circle.Radius || projectedDistance < 0)
            continue;

        float distanceClosestPointFromStart =
            glm::sqrt(distanceToStartSquared + glm::pow(projectedDistance, 2));
        float distanceClosestPointFromEnd =
            glm::sqrt(distanceToEndSquared + glm::pow(projectedDistance, 2));

        float lineLength = glm::distance(line.Start, line.End);

        if (distanceClosestPointFromStart > lineLength ||
            distanceClosestPointFromEnd > lineLength)
            continue;

        intersectionDepth = circle.Radius - projectedDistance;
        ResolveCollision(circle, line, intersectionDepth);
    }
}

void Assignment3::ResolveCollision(Circle& circle, Line line, float intersectionDepth) {
    circle.Position += intersectionDepth * line.Normal;
    circle.Velocity -= 2.0f * glm::dot(circle.Velocity, line.Normal) * line.Normal;
}
