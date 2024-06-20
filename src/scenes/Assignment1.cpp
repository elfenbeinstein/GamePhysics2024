#include "Assignment1.h"

#include "core/Draw.h"
#include "imgui.h"
#include "geometry/Circle.h"
#include "geometry/Line.h"
#include <random>

Assignment1::Assignment1()
    : nextCirclePosition(0, 3),
      nextCircleRadius(0.5),
      nextCircleVelocity(0, 0),
      circles(),
      verticalLines(),
      horizontalLines() 
{
    std::random_device rd;
    gen = std::mt19937(rd());

    verticalLines.push_back(Line::Line({-18.0f, 0.0f}, {-18.0f, 9.0f}));
    verticalLines.push_back(Line::Line({-15.0f, -9.0f}, {-15.0f, 0.0f}));
    verticalLines.push_back(Line::Line({-9.0f, 9.0f}, {-9.0f, 18.0f}));
    verticalLines.push_back(Line::Line({0.0f, -9.0f}, {0.0f, -1.5f}));
    verticalLines.push_back(Line::Line({4.5f, -6.5f}, {4.5f, -1.5f}));
    verticalLines.push_back(Line::Line({18.0f, -6.5f}, {18.0f, 18.0f}));

    horizontalLines.push_back(Line::Line({-18.0f, 0.0f}, {-15.0f, 0.0f}));
    horizontalLines.push_back(Line::Line({-18.0f, 9.0f}, {-9.0f, 9.0f}));
    horizontalLines.push_back(Line::Line({-15.0f, -9.0f}, {0.0f, -9.0f}));
    horizontalLines.push_back(Line::Line({-9.0f, 18.0f}, {18.0f, 18.0f}));
    horizontalLines.push_back(Line::Line({0.0f, -1.5f}, {4.5f, -1.5f}));
    horizontalLines.push_back(Line::Line({4.5f, -6.5f}, {18.0f, -6.5f}));


}

Assignment1::~Assignment1() {}

void Assignment1::OnEnable() {}

void Assignment1::OnDisable() {}

void Assignment1::Update(float deltaTime) {
    
    for (auto& circle : circles) {
        circle.Velocity = circle.Velocity + g * deltaTime;
        circle.Position = circle.Position + circle.Velocity * deltaTime;

        //first try, thought the other way might be easier to read, but left in for reference
        /*if (CollidedWithHorizontalLine(circle)) {
            circle.Velocity.y *= -1.0f;
        }
        if (CollidedWithVerticalLine(circle)) {
            circle.Velocity.x *= -1.0f;
        }*/

        //second try, now also resets the circle position a little better if collided with end points
        CheckForCollisions(circle);
    }
}

void Assignment1::Draw() {
    for (const auto& circle : circles) {
        Draw::Circle(circle.Position, circle.Radius);
    }

    for (const auto& line : horizontalLines) {
        Draw::Line(line.Position, line.EndOffset);
    }

    for (const auto& line : verticalLines) {
        Draw::Line(line.Position, line.EndOffset);
    }
}

void Assignment1::DrawGUI() {
    ImGui::Begin("Inspector");
    ImGui::Text("Next Circle Values:");
    ImGui::DragFloat2("Position", &nextCirclePosition[0], 0.1f, -10.0f, 10.0f);
    ImGui::DragFloat("Radius", &nextCircleRadius, 0.1f, 0.01f, 5.0f);
    ImGui::DragFloat2("Velocity", &nextCircleVelocity[0], 0.1f);

    ImGui::Spacing();
    if (ImGui::Button("Add New Circle")) {
        circles.push_back(
            Circle(nextCirclePosition, nextCircleVelocity, nextCircleRadius));
    }

    ImGui::Spacing();
    if (ImGui::Button("Add Random Circle")) {
        circles.push_back(Assignment1::CreateRandomCircle());
    }

    ImGui::Spacing();
    if (ImGui::Button("Remove all Circles")) {
        circles.clear();
    }

    ImGui::End();
}

Circle Assignment1::CreateRandomCircle() {
    float radius = std::uniform_real_distribution<float>(0.3f, 3.0f)(gen);

    std::uniform_real_distribution<float> p(0.0f, 5.0f);
    float x = p(gen);
    float y = p(gen);
    glm::vec2 pos = {x, y};

    std::uniform_real_distribution<float> v(-10.0f, 10.0f);
    float vx = v(gen);
    float vy = v(gen);
    glm::vec2 vel = {vx, vy};

    return Circle::Circle(pos, vel, radius);
}

void Assignment1::CheckForCollisions(Circle& circle) {
    for (const auto& line : horizontalLines) {
        if (!CollidesWithInfinite(circle.Position.y, line.EndOffset.y, circle.Radius))
            continue;
        
        bool collidesWithStart = CollidesWithEndpoint(circle.Position, line.Position, circle.Radius);
        bool collidesWithEnd = CollidesWithEndpoint(circle.Position, line.EndOffset, circle.Radius);
        
        if (!IsBetweenEndpoints(circle.Position.x, line.Position.x, line.EndOffset.x) 
            && !collidesWithEnd && !collidesWithStart)
            continue;

        // still a little wonky reset, but best try so far
        if (collidesWithEnd || collidesWithStart) {
            float distanceCrossed = collidesWithStart
                    ? std::abs(circle.Radius - VectorDistance(circle.Position, line.Position))
                    : std::abs(circle.Radius - VectorDistance(circle.Position, line.EndOffset));

            glm::vec2 direction = glm::normalize(circle.Velocity);
            glm::vec2 moveBackVector = direction * distanceCrossed;
            circle.Position -= moveBackVector;
        } else {
            circle.Position.y = circle.Velocity.y >= 0
                                    ? line.EndOffset.y - circle.Radius
                                    : line.EndOffset.y + circle.Radius;
        }

        circle.Velocity.y *= -1.0f;

        break;
    }

    for (const auto& line : verticalLines) {
        if (!CollidesWithInfinite(circle.Position.x, line.EndOffset.x, circle.Radius))
            continue;
        
        bool collidesWithStart = CollidesWithEndpoint(circle.Position, line.Position, circle.Radius);
        bool collidesWithEnd = CollidesWithEndpoint(circle.Position, line.EndOffset, circle.Radius);
        
        if (!IsBetweenEndpoints(circle.Position.y, line.Position.y, line.EndOffset.y) &&
            !collidesWithEnd && !collidesWithStart)
            continue;

        //still a little wonky reset but best try so far
        if (collidesWithEnd || collidesWithStart) {
            float distanceCrossed = collidesWithStart
                    ? std::abs(circle.Radius - VectorDistance(circle.Position, line.Position))
                    : std::abs(circle.Radius - VectorDistance(circle.Position, line.EndOffset));

            glm::vec2 direction = glm::normalize(circle.Velocity);
            glm::vec2 moveBackVector = direction * distanceCrossed;
            circle.Position -= moveBackVector;
        } else {
            circle.Position.x = circle.Velocity.x >= 0
                                    ? line.EndOffset.x - circle.Radius
                                    : line.EndOffset.x + circle.Radius;
        }
        
        circle.Velocity.x *= -1.0f;

        break;
    }
}


bool Assignment1::CollidedWithHorizontalLine(Circle& circle) {
    for (const auto& line : horizontalLines) {
        if (std::abs(circle.Position.y - line.EndOffset.y) > circle.Radius)
            continue;

        // first try: but was really wonky in corners
        /*if (circle.Position.x <= line.Start.x - circle.Radius ||
            circle.Position.x >= line.End.x + circle.Radius)
            continue;*/

        if (circle.Position.x < line.Position.x &&
            VectorDistance(circle.Position, line.Position) > circle.Radius)
            continue;

        if (circle.Position.x > line.EndOffset.x && 
            VectorDistance(circle.Position, line.EndOffset) > circle.Radius)
            continue;

        // resetting is wonky if it hits endpoints in specific ways
        circle.Position.y = circle.Velocity.y >= 0 ? line.EndOffset.y - circle.Radius
                                                   : line.EndOffset.y + circle.Radius;

        return true;
    }
    return false;
}

bool Assignment1::CollidedWithVerticalLine(Circle& circle) {
    for (const auto& line : verticalLines) {
        if (std::abs(circle.Position.x - line.EndOffset.x) > circle.Radius)
            continue;

        //first try: but was really wonky in corners
        /*if (circle.Position.y <= line.Position.y - circle.Radius ||
            circle.Position.y >= line.EndOffset.y + circle.Radius)
            continue;*/

        if (circle.Position.y < line.Position.y &&
            VectorDistance(circle.Position, line.Position) >
                circle.Radius)
            continue;

        if (circle.Position.y > line.EndOffset.y &&
            VectorDistance(circle.Position, line.EndOffset) >
                circle.Radius)
            continue;

        // resetting is wonky if it hits endpoints in specific ways
        circle.Position.x = circle.Velocity.x >= 0 ? line.EndOffset.x - circle.Radius
                                                   : line.EndOffset.x + circle.Radius;

        return true;
    }
    return false;
}

float Assignment1::VectorDistance(glm::vec2 v1, glm::vec2 v2) {
    return std::sqrt(std::pow(v2.x - v1.x, 2) + std::pow(v2.y - v1.y, 2));
}

bool Assignment1::CollidesWithInfinite(float circle, float line, float radius) {
    if (std::abs(circle - line) > radius) return false;
    return true;
}

bool Assignment1::IsBetweenEndpoints(float circle, float start, float end) {
    if (circle < start || circle > end) return false;
    return true;
}

bool Assignment1::CollidesWithEndpoint(glm::vec2 circle, glm::vec2 endpoint, float radius) {
    return VectorDistance(circle, endpoint) <= radius;
}
