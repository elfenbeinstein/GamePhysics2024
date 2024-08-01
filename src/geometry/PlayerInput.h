#pragma once
#include "core/Simple2DScene.h"
#include <vector>
#include "geometry/Particle.h"
#include "geometry/Circle.h"
#include "geometry/Rectangle.h"
#include "geometry/Line.h"
#include <memory>

class PlayerInput {
public:
    enum InputType { None, Impulse, Move, DrawRect, DrawCircle, DrawLine };
    PlayerInput();
    PlayerInput(InputType leftMouseInput);
    void Draw();
    void Update(std::vector<std::shared_ptr<Particle>>& particles);
    std::shared_ptr<Particle> selectedParticle;
    InputType LeftMouseInputType;
    int PreviewSteps;
    float PreviewTime;

private:
    bool leftMouseDown;
    glm::vec2 mousePosition;
    glm::vec2 originalPosition;
    std::vector<std::shared_ptr<Particle>> previewParticles;
    Color originalColour;

    void MouseDown(const InputType& inputType, std::shared_ptr<Particle>& particle, const glm::vec2 prevMousePosition);
    void MouseReleased(const InputType& inputType, std::shared_ptr<Particle>& particle);
    void MouseClicked(const InputType& inputType, std::shared_ptr<Particle>& particle);
    void CalculateTrajectory();
};