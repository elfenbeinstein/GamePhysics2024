#include "Assignment4.h"

#include "imgui.h"
#include <random>
#include "core/Draw.h"
#include "core/Colors.h"

Assignment4::Assignment4() : lines(), circles(), takeMassIntoAccount(true) {
    std::random_device rd;
    gen = std::mt19937(rd());

    lines.push_back(Line::Line({-12.5f, 0.0f}, {-7.5f, -2.5f}));
    lines.push_back(Line::Line({-7.5f, -2.5f}, {-5.0f, -5.0f}));
    lines.push_back(Line::Line({-5.0f, -5.0f}, {-2.5f, -5.0f}));
    lines.push_back(Line::Line({-2.5f, -5.0f}, {2.5f, -7.5f}));
    lines.push_back(Line::Line({2.5f, -7.5f}, {8.5f, -7.5f}));
    lines.push_back(Line::Line({8.5f, -7.5f}, {12.5f, 0.0f}));
    lines.push_back(Line::Line({12.5f, 0.0f}, {12.5f, 4.5f}));
    lines.push_back(Line::Line({12.5f, 4.5f}, {5.0f, 8.5f}));
    lines.push_back(Line::Line({5.0f, 8.5f}, {-7.5f, 8.5f}));
    lines.push_back(Line::Line({-7.5f, 8.5f}, {-7.5f, 4.5f}));
    lines.push_back(Line::Line({-7.5f, 4.5f}, {-10.0f, 4.5f}));
    lines.push_back(Line::Line({-10.0f, 4.5f}, {-12.5f, 0.0f}));
}

Assignment4::~Assignment4() {}

void Assignment4::OnEnable() {}

void Assignment4::OnDisable() {}

void Assignment4::Update(float deltaTime) {
    for (auto& circle : circles) {
        if (circle.Colour == Colors::red) {
            circle.ColorCountdown += deltaTime;
            if (circle.ColorCountdown > 0.3f)
                circle.Colour = Colors::white;
        }
    }

    for (std::size_t i = 0; i < circles.size(); ++i) {
        auto& circle = circles[i];
        circle.TotalForce = glm::vec2(0);

        AddForce(circle, g);

        circle.Acceleration = circle.TotalForce * GetInverseMass(circle.Mass);
        circle.Velocity += circle.Acceleration * deltaTime;
        circle.Position += circle.Velocity * deltaTime;

        for (std::size_t j = i + 1; j < circles.size(); ++j) {
            if (takeMassIntoAccount)
                CheckForCollisionBetweenCirclesTakingMassIntoAccount(circle, circles[j]);
            else
                CheckForCollisionBetweenCircles(circle, circles[j]);
        }

        CheckForCollisionWithLines(circle);
    }
}

void Assignment4::Draw() {
    Draw::SetColor(Colors::gray);
    Draw::Line(glm::vec2{-15.0f, 0.0f}, glm::vec2{15.0f, 0.0f});
    Draw::Line(glm::vec2{0.0f, 15.0f}, glm::vec2{0.0f, -15.0f});
    Draw::SetColor(Colors::white);

    for (const auto& circle : circles) {
        Draw::SetColor(circle.Colour);
        Draw::Circle(circle.Position, circle.Radius);
    }
    Draw::SetColor(Colors::white);

    for (const auto& line : lines) {
        Draw::Line(line.Position, line.End);
    }
}

void Assignment4::DrawGUI() {
    ImGui::Begin("Inspector");
    
    if (ImGui::Button("Add Random Circle")) {
        circles.push_back(CreateRandomCircle(false));
    }
    if (ImGui::Button("Add Static Circle")) {
        circles.push_back(CreateRandomCircle(true));
    }
    if (ImGui::Button("Remove All Circles")) {
        circles.clear();
    }
    ImGui::Checkbox("Take Mass Into Account", &takeMassIntoAccount);

    ImGui::End();
}

Circle Assignment4::CreateRandomCircle(bool isStatic) {
    //float radius = std::uniform_real_distribution<float>(0.2f, 1.0f)(gen);

    std::uniform_real_distribution<float> p(-2.5f, 2.5f);
    float x = p(gen);
    float y = p(gen);
    glm::vec2 pos = {x, y};

    std::uniform_real_distribution<float> v(-10.0f, 10.0f);
    float vx = v(gen);
    float vy = v(gen);
    glm::vec2 vel = {vx, vy};

    float mass = std::uniform_real_distribution<float>(0.05f, 5.0f)(gen);
    float radius =
        0.2f + (mass - 0.05f) * ((1.0f - 0.2f) / (5.0f - 0.05f));

    if (isStatic) {
        mass = 0.0f;
        vel = {0.0f, 0.0f};
    }

    return Circle::Circle(pos, vel, radius, mass);
}

void Assignment4::AddForce(Circle& circle, const glm::vec2& force) {
    circle.TotalForce += force * circle.Mass;
}

float Assignment4::GetInverseMass(float mass) {
    if (mass == 0.0f)
        return 0.0f;
    return 1.0f / mass;
}

void Assignment4::CheckForCollisionWithLines(Circle& circle) {
    for (const auto& line : lines) {
        float radiusSquared = std::pow(circle.Radius, 2);
        float intersectionDepth = 0.0f;

        float distanceToStartSquared =
            std::pow(line.Position.x - circle.Position.x, 2) +
            std::pow(line.Position.y - circle.Position.y, 2);
        if (distanceToStartSquared <= radiusSquared) {
            intersectionDepth =
                std::sqrt(radiusSquared - distanceToStartSquared);
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

        if (projectedDistance > circle.Radius || projectedDistance < 0)
            continue;

        float distanceClosestPointFromStart =
            glm::sqrt(distanceToStartSquared + glm::pow(projectedDistance, 2));
        float distanceClosestPointFromEnd =
            glm::sqrt(distanceToEndSquared + glm::pow(projectedDistance, 2));

        float lineLength = glm::distance(line.Position, line.End);

        if (distanceClosestPointFromStart > lineLength ||
            distanceClosestPointFromEnd > lineLength)
            continue;

        intersectionDepth = circle.Radius - projectedDistance;
        ResolveCollision(circle, line, intersectionDepth);
    }
}

void Assignment4::ResolveCollision(Circle& circle, Line line, float intersectionDepth) {
    circle.Position += intersectionDepth * line.Normal;
    circle.Velocity -= 2.0f * glm::dot(circle.Velocity, line.Normal) * line.Normal;
}

void Assignment4::CheckForCollisionBetweenCircles(Circle& circle1, Circle& circle2) {
    float distance =
        std::sqrt(std::pow(circle2.Position.x - circle1.Position.x, 2) +
                  std::pow(circle2.Position.y - circle1.Position.y, 2));
    if (distance > circle1.Radius + circle2.Radius)
        return;
    
    circle1.Colour = Colors::red;
    circle1.ColorCountdown = 0.0f;
    circle2.Colour = Colors::red;
    circle2.ColorCountdown = 0.0f;

    float intersectionDepth = circle1.Radius + circle2.Radius - distance;
    if (intersectionDepth > 0) {
        glm::vec2 direction(
            (circle2.Position.x - circle1.Position.x) / distance,
            (circle2.Position.y - circle1.Position.y) / distance);
        if (circle1.Mass > 0.0f) 
            circle1.Position -= direction * (intersectionDepth / 2.0f + 0.05f);
        if (circle2.Mass > 0.0f)
            circle2.Position += direction * (intersectionDepth / 2.0f + 0.05f);
    }

    glm::vec2 middle =
        glm::vec2((circle1.Position.x + circle2.Position.x) / 2.0f,
                  (circle1.Position.y + circle2.Position.y) / 2.0f);
    glm::vec2 normal1 = glm::normalize(glm::vec2(circle1.Position.x - middle.x,
                                                circle1.Position.y - middle.y));
    glm::vec2 normal2 = glm::normalize(glm::vec2(circle2.Position.x - middle.x,
                                                circle2.Position.y - middle.y));
    circle1.Velocity =
        circle1.Velocity - 2.0f * glm::dot(circle1.Velocity, normal1) * normal1;
    circle2.Velocity =
        circle2.Velocity - 2.0f * glm::dot(circle2.Velocity, normal2) * normal2;
}

// attempt to calculate new velocity while taking relative mass into account
// not sure this works correctly, it seems a little wonky, but did not know how to do it better
void Assignment4::CheckForCollisionBetweenCirclesTakingMassIntoAccount(
    Circle& circle1,
    Circle& circle2) {
    float distance =
        std::sqrt(std::pow(circle2.Position.x - circle1.Position.x, 2) +
                  std::pow(circle2.Position.y - circle1.Position.y, 2));
    if (distance > circle1.Radius + circle2.Radius)
        return;

    circle1.Colour = Colors::red;
    circle1.ColorCountdown = 0.0f;
    circle2.Colour = Colors::red;
    circle2.ColorCountdown = 0.0f;

    float totalMass = circle1.Mass + circle2.Mass;
    if (totalMass < 0.0001f)
        return;

    float massRatio1 = circle1.Mass / totalMass;
    float massRatio2 = circle2.Mass / totalMass;

    float intersectionDepth = circle1.Radius + circle2.Radius - distance;
    glm::vec2 direction((circle2.Position.x - circle1.Position.x) / distance,
                        (circle2.Position.y - circle1.Position.y) / distance);

    if (intersectionDepth > 0) {
        if (circle2.Mass < 0.0001f) {
            circle1.Position -= direction * intersectionDepth;
        } else if (circle1.Mass < 0.0001f) {
            circle2.Position += direction * intersectionDepth;
        } else {
            circle1.Position -= direction * (intersectionDepth * massRatio2);
            circle2.Position += direction * (intersectionDepth * massRatio1);
        }
    }
    
    if (circle1.Mass == 0.0f) {
        glm::vec2 middle =
            glm::vec2((circle1.Position.x + circle2.Position.x) / 2.0f,
                      (circle1.Position.y + circle2.Position.y) / 2.0f);
         glm::vec2 normal2 = glm::normalize(glm::vec2(
             circle2.Position.x - middle.x, circle2.Position.y - middle.y));
         circle2.Velocity =
             circle2.Velocity -
             2.0f * glm::dot(circle2.Velocity, normal2) * normal2;
        return;
    }
    if (circle2.Mass == 0.0f) {
        glm::vec2 middle =
            glm::vec2((circle1.Position.x + circle2.Position.x) / 2.0f,
                      (circle1.Position.y + circle2.Position.y) / 2.0f);
        glm::vec2 normal1 = glm::normalize(glm::vec2(
            circle1.Position.x - middle.x, circle1.Position.y - middle.y));
        circle1.Velocity = circle1.Velocity -
                           2.0f * glm::dot(circle1.Velocity, normal1) * normal1;
        return;
    }

    glm::vec2 relativeVelocity = circle2.Velocity - circle1.Velocity;
    float dotProduct = glm::dot(relativeVelocity, direction);
    float impulse = 2.0f * dotProduct / totalMass;

    circle1.Velocity += impulse * circle2.Mass * direction;
    circle2.Velocity -= impulse * circle1.Mass * direction;
}