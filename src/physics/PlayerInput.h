#pragma once
#include "core/Simple2DScene.h"
#include <vector>
#include "physics/Particle.h"
#include "physics/Circle.h"
#include "physics/Rectangle.h"
#include "physics/Line.h"
#include "physics/AxisAlignedBox.h"
#include <memory>

class PlayerInput {
public:
    enum InputType { None, Impulse, Move, DrawRect, DrawCircle, DrawLine, DrawAABB, RemoveParticle};
    PlayerInput();
    PlayerInput(InputType leftMouseInput, bool applyImpulsiveTorque);
    void Draw();
    void Update(std::vector<std::shared_ptr<Particle>>& particles);
    std::shared_ptr<Particle> selectedParticle;
    InputType LeftMouseInputType;
    int PreviewSteps;
    float PreviewTime;

private:
    bool leftMouseDown;
    bool ApplyImpulsiveTorque;
    glm::vec2 mousePosition;
    glm::vec2 originalPosition;
    std::vector<std::shared_ptr<Particle>> previewParticles;
    Color originalColour;

    void MouseDown(const InputType& inputType, std::shared_ptr<Particle>& particle, const glm::vec2 prevMousePosition);
    void MouseReleased(const InputType& inputType, std::shared_ptr<Particle>& particle);
    void MouseClicked(const InputType& inputType, std::shared_ptr<Particle>& particle);
    void CalculateTrajectory();
};