#include "Assignment2.h"

#include "core/Draw.h"
#include "core/Colors.h"
#include "physics/Circle.h"
#include "imgui.h"
#include <iostream>
#include <vector>

Assignment2::Assignment2()
    : nextCirclePosition(0, 3),
      nextCircleRadius(0.5),
      nextCircleMass(1.0f),
      circles(),
      trajectory(),
      impulseCircleIndex(-1),
      mousePosition(),
      mouseDown(false),
      showTrajectory(true),
      steps(50),
      previewTime(4.0f),
      trajectoryMethod(5),
      distanceAfterThrowOnGround(-1.0f),
      firstCollisionTime(0),
      collisionPosition() {}

Assignment2::~Assignment2() {}

void Assignment2::OnEnable() {}

void Assignment2::OnDisable() {}

void Assignment2::Update(float deltaTime) {
    mousePosition = Input::GetMousePos();

    if (Input::IsMouseClicked(0)) {
        impulseCircleIndex = SetCurrentImpulseCircle();
        if (impulseCircleIndex != -1)
            mouseDown = true;
    }
    if (Input::IsMouseReleased(0)) {
        if (mouseDown && impulseCircleIndex != -1 && impulseCircleIndex < circles.size()) {
            AddImpulse(circles[impulseCircleIndex],
                       mousePosition - circles[impulseCircleIndex].Position);
        }
        mouseDown = false;
        impulseCircleIndex = -1;
    }

    //calculate movement if mouse is not down for circles
    for (std::size_t i = 0; i < circles.size(); ++i) {
        if (i == impulseCircleIndex)
            continue;
        auto& circle = circles[i];
        circle.TotalForce = glm::vec2(0);

        AddForce(circle, g);

        circle.Acceleration = circle.TotalForce * GetInverseMass(circle.Mass);
        circle.Velocity += circle.Acceleration * deltaTime;
        circle.Position += circle.Velocity * deltaTime;

        if (circle.Position.y - circle.Radius <= 0) {
            circle.Position.y = circle.Radius;
            circle.Velocity.y *= -1.0f;
        }
    }
}

void Assignment2::Draw() {
    for (const auto& circle : circles) {
        Draw::Circle(circle.Position, circle.Radius);
    }

    Draw::Line(glm::vec2(-100, 0), glm::vec2(100, 0));
    
    if (mouseDown && impulseCircleIndex != -1 && impulseCircleIndex < circles.size()) {
        Draw::SetColor(Colors::red);
        Draw::Arrow(circles[impulseCircleIndex].Position, mousePosition);
        Draw::SetColor(Colors::gray);
        // simple method without collision detection
        if (trajectoryMethod == 0)
            DrawTrajectory(2.0f, mousePosition - circles[impulseCircleIndex].Position, 20); 
        else
            DrawTrajectoryWithGroundCollision(previewTime, mousePosition - circles[impulseCircleIndex].Position, steps);
        Draw::SetColor(Colors::white);
    }

    if (!showTrajectory || circles.size() == 0)
        return;
    Draw::SetColor(Colors::gray);
    for (const auto& position : trajectory) {
        Draw::Circle(position, circles[0].Radius);
    }
    Draw::SetColor(Colors::white);
}

void Assignment2::DrawGUI() {
    ImGui::Begin("Inspector");
    ImGui::DragFloat2("Next Position", &nextCirclePosition[0], 0.1f);
    ImGui::DragFloat("Next Mass", &nextCircleMass, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat("Next Radius", &nextCircleRadius, 0.1f);
    if (ImGui::Button("Add new Circle")) {
        circles.push_back(Circle(nextCirclePosition, {0, 0}, nextCircleRadius, nextCircleMass));
    }
    if (ImGui::Button("Remove all Circles")) {
        circles.clear();
        trajectory.clear();
        mouseDown = false;
        impulseCircleIndex = -1;
    }
    ImGui::Spacing();
    ImGui::Checkbox("Show Trajectory", &showTrajectory);
    ImGui::DragInt("Steps", &steps, 0.1f, 0, 500);
    ImGui::DragFloat("Preview Time", &previewTime, 0.1f, 1.0f, 10.0f);
    ImGui::DragInt("Calculation Method", &trajectoryMethod, 0.1f, 0, 5);
    /*if (distanceAfterThrowOnGround >= 0) {
        ImGui::Text("First Collision with Ground:");
        ImGui::Text("Distance From Original Position: %.2f", distanceAfterThrowOnGround);
        ImGui::Text("Time: %.2f", firstCollisionTime);
        ImGui::Text("X Position: %.2f", collisionPosition.x);
    }*/

    ImGui::End();
}

void Assignment2::AddImpulse(Circle& circle, const glm::vec2& impulse) {
    circle.Velocity += impulse * GetInverseMass(circle.Mass);
}
void Assignment2::AddForce(Circle& circle, const glm::vec2& force) {
    circle.TotalForce += force * circle.Mass;
}

void Assignment2::DrawTrajectory(float previewTime, glm::vec2 impulse, int steps) {
    if (impulseCircleIndex == -1 || impulseCircleIndex >= circles.size())
        return;
    Circle& circle = circles[impulseCircleIndex];
    trajectory.clear();

    for (int i = 0; i < steps; ++i) {
        float timeStep = previewTime / steps * i;
        float powerTo2 = glm::pow(timeStep, 2);
        float inverseMass = GetInverseMass(circle.Mass);
        glm::vec2 velocity = circle.Velocity + impulse * inverseMass;
        glm::vec2 acceleration = circle.TotalForce * inverseMass;
        glm::vec2 pos = circle.Position + velocity * timeStep + 0.5f * acceleration * powerTo2;
        if (pos.y < 0 + circle.Radius)
            return;
        Draw::Circle(pos, circle.Radius);
        trajectory.push_back(pos);
    }
}

void Assignment2::DrawTrajectoryWithGroundCollision(float previewTime, glm::vec2 impulse, int steps) {
    if (impulseCircleIndex == -1 || impulseCircleIndex >= circles.size())
        return;
    trajectory.clear();
    Circle circle = circles[impulseCircleIndex];
    float inverseMass = GetInverseMass(circle.Mass);

    circle.Velocity = circle.Velocity + impulse * inverseMass;
    circle.Acceleration = circle.TotalForce * inverseMass;

    float intersectionTime = GetNextIntersectionTime(previewTime, circle, impulse, inverseMass);
    distanceAfterThrowOnGround = DistanceFromOriginalPositionWhenCircleHitsGround(circle.Position, circle.Velocity, circle.Acceleration, intersectionTime);
    float deltaTime = previewTime / steps;
    
    for (int i = 0; i < steps; ++i) {
        // FIRST ATTEMPT
        // this worked only until first collision with the ground, then things occasionally went haywire - couldn't get it to work properly, very buggy
        // i do like the idea though of pre-calculating the next intersection time (next collision time if you will)
        // but resetting the variables after the first collision didn't work properly
        if (trajectoryMethod == 1) {
            float timeStep = previewTime / steps * i;

            if (timeStep >= intersectionTime) {
                float powerTo2 = glm::pow(intersectionTime, 2);
                glm::vec2 collisionPosition = circle.Position + circle.Velocity * intersectionTime + 0.5f * circle.Acceleration * powerTo2;

                Draw::Circle(collisionPosition, circle.Radius); // --> here collision with ground happens
                trajectory.push_back(collisionPosition);

                circle.Position = collisionPosition;
                circle.Velocity = circle.Velocity + circle.Acceleration * intersectionTime;
                circle.Velocity.y *= -1.0f;

                previewTime = previewTime - intersectionTime;
                steps -= i;
                if (steps < 0)
                    return;
                i = 0;
                timeStep = previewTime / steps * i;

                intersectionTime = GetNextIntersectionTime( previewTime, circle, impulse, inverseMass);
            }

            float powerTo2 = glm::pow(timeStep, 2);
            glm::vec2 pos = circle.Position + circle.Velocity * timeStep +
                            0.5f * circle.Acceleration * powerTo2;
            if (pos.y < 0 + circle.Radius)
                pos.y = circle.Radius;
            Draw::Circle(pos, circle.Radius);
            trajectory.push_back(pos);
        }

        // SECOND ATTEMPT 
        // this worked better - but only an approximation of when it hits the
        // ground, depending on preview time and steps very inaccurate: the more
        // steps in preview time the more accurate it seemed to be
        // use show trajectory to see innacuracies
        if (trajectoryMethod == 2) {
            circle.Velocity += circle.Acceleration * deltaTime;
            circle.Position += circle.Velocity * deltaTime;
            if (circle.Position.y - circle.Radius <= 0) {
                circle.Position.y = circle.Radius;
                circle.Velocity.y *= -1.0f;
            }
            Draw::Circle(circle.Position, circle.Radius);
            trajectory.push_back(circle.Position);
        }

        // THIRD ATTEMPT - combining 1 and 2
        // super buggy, does not work -- one problem was the intersection time calculation was faulty, I fixed this after the third attempt
        // problem with calc --> see intersection method
        if (trajectoryMethod == 3) {
            if (deltaTime * i >= intersectionTime) {
                circle.Velocity += circle.Acceleration * (deltaTime * i -
            intersectionTime); circle.Position += circle.Velocity * (deltaTime *
            i - intersectionTime);

                Draw::Circle(circle.Position, circle.Radius);
                trajectory.push_back(circle.Position);
                circle.Velocity.y *= -1.0f;

                float nextIntersectionTime = GetNextIntersectionTime(previewTime
            - intersectionTime, circle, impulse, inverseMass);

                float deltaAfterCollision = deltaTime * i - intersectionTime;
                circle.Velocity += circle.Acceleration * deltaAfterCollision;
                circle.Position += circle.Velocity * deltaAfterCollision;
                Draw::Circle(circle.Position, circle.Radius);

                intersectionTime = nextIntersectionTime;

                continue;
            }

            circle.Velocity += circle.Acceleration * deltaTime;
            circle.Position += circle.Velocity * deltaTime;

            Draw::Circle(circle.Position, circle.Radius);
            trajectory.push_back(circle.Position);
        }

        // SOLUTION:
        if (trajectoryMethod == 4 || trajectoryMethod == 5) {
            float timeStep = previewTime / steps * i;

            if (timeStep >= intersectionTime && intersectionTime > 0) {
                float intersectionPowerTo2 = glm::pow(intersectionTime, 2);
                glm::vec2 collisionPosition =
                    circle.Position + circle.Velocity * intersectionTime +
                    0.5f * circle.Acceleration * intersectionPowerTo2;

                if (!showTrajectory)
                    Draw::Circle(collisionPosition, circle.Radius);
                trajectory.push_back(collisionPosition);

                circle.Position = collisionPosition;
                circle.Velocity =
                    circle.Velocity + circle.Acceleration * intersectionTime;
                // for some reason the velocity.y was SOMETIMES already positive, so multiplying by -1.0f meant it continued through the ground
                // i am unsure why that was the case, but this seems to work perfectly before (when just multiplying by -1) 
                // it was hella buggy and depending on the value of the impulse was all over the place -- test with method 4
                if (trajectoryMethod == 4) {
                    circle.Velocity.y *= -1.0f;
                } else {
                    if (circle.Velocity.y < 0)
                        circle.Velocity.y *= -1.0f;
                }

                previewTime = previewTime - intersectionTime;
                steps -= i;
                if (steps < 0)
                    return;
                i = 0;
                intersectionTime = GetNextIntersectionTime(
                    previewTime, circle, impulse, inverseMass);
                continue;
            }

            float powerTo2 = glm::pow(timeStep, 2);
            glm::vec2 pos = circle.Position + circle.Velocity * timeStep +
                            0.5f * circle.Acceleration * powerTo2;
            if (!showTrajectory)
                Draw::Circle(pos, circle.Radius);
            trajectory.push_back(pos);
        }
    }
}

int Assignment2::SetCurrentImpulseCircle() {
    for (std::size_t i = 0; i < circles.size(); ++i) {
        float distance = glm::distance(mousePosition, circles[i].Position);
        if (distance <= circles[i].Radius)
            return i;
    }
    return -1;
}

float Assignment2::GetInverseMass(float mass) {
    if (mass == 0.0f)
        return 0.0f;
    return 1.0f / mass;
}

float Assignment2::GetNextIntersectionTime(float previewTime, Circle& circle, glm::vec2 impulse, float inverseMass) {
    float acceleration = circle.TotalForce.y * inverseMass;
    // this was the problem, I added the impulse twice, so after the first collision (second calculation) the values were off
    float startVelocity = circle.Velocity.y /*+ impulse.y * inverseMass*/; 
    float startYPosition = circle.Position.y;

    float discriminant = startVelocity * startVelocity -
                  2.0f * acceleration * (startYPosition - circle.Radius);
    if (discriminant < 0) {
        return -1.0f;
    } else {
        float t1 = (-startVelocity + std::sqrt(discriminant)) / (1.0f * acceleration);
        float t2 = (-startVelocity - std::sqrt(discriminant)) / (1.0f * acceleration);

        if (t1 > 0 && t2 > 0) {
            return std::min(t1, t2);
        } else if (t1 > 0) {
            return t1;
        } else if (t2 > 0) {
            return t2;
        } else {
            return -1.0f;
        }
    }
}

float Assignment2::DistanceFromOriginalPositionWhenCircleHitsGround(glm::vec2 startPosition, glm::vec2 startVelocity, glm::vec2 acceleration, float collisionTime) {
    float powerTo2 = glm::pow(collisionTime, 2);
    collisionPosition = startPosition + startVelocity * collisionTime +
                        0.5f * acceleration * powerTo2;
    firstCollisionTime = collisionTime;
    return std::sqrt(std::pow(startPosition.x - collisionPosition.x, 2) +
                     std::pow(startPosition.y - collisionPosition.y, 2));
}
