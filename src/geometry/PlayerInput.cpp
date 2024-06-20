#include "PlayerInput.h"
#include "core/Draw.h"

PlayerInput::PlayerInput()
    : LeftMouseInputType(None),
      leftMouseDown(false),
      mousePosition(),
      originalPosition(),
      selectedParticle(nullptr),
      originalColour() {}

PlayerInput::PlayerInput(InputType leftMouseInput)
    : LeftMouseInputType(leftMouseInput),
      leftMouseDown(false),
      mousePosition(),
      originalPosition(),
      selectedParticle(nullptr),
      originalColour() {}

void PlayerInput::Draw() {
    if (trajectoryParticles.size() == 0)
        return;
    for (auto& particle : trajectoryParticles) {
        particle->Draw();
    }
}

void PlayerInput::Update(std::vector<std::shared_ptr<Particle>>& particles) {
    if (LeftMouseInputType == None)
        return;

    glm::vec2 previousMousePosition = mousePosition;
    mousePosition = Input::GetMousePos();

    if (LeftMouseInputType != None) {
        if (Input::IsMouseClicked(0)) {
            for (std::size_t i = 0; i < particles.size(); ++i) {
                if (!particles[i]->IsMouseOverParticle(mousePosition))
                    continue;
                if (LeftMouseInputType == Impulse && !particles[i]-> CanAddImpulseByMouse)
                    continue;
                if (LeftMouseInputType == Move && !particles[i]->CanBeMovedByMouse)
                    continue;
                leftMouseDown = true;
                selectedParticle = particles[i];
                MouseClicked(LeftMouseInputType, selectedParticle);
                originalColour = selectedParticle->Colour;
                selectedParticle->Colour = Colors::cyan;
                break;
            }
        }
        if (Input::IsMouseReleased(0)) {
            MouseReleased(LeftMouseInputType, selectedParticle);
            if (selectedParticle != nullptr) 
                selectedParticle->Colour = originalColour;
            selectedParticle = nullptr;
        }
        if (leftMouseDown) {
            MouseDown(LeftMouseInputType, selectedParticle, previousMousePosition);
        }
    }
}

void PlayerInput::MouseDown(const InputType& inputType, std::shared_ptr<Particle>& particle, const glm::vec2 prevMousePosition) {
    if (particle == nullptr)
        return;

    if (inputType == Move) {
        particle->Move(prevMousePosition, mousePosition);
    } else if (inputType == Impulse) {
        CalculateTrajectory();
    }
}

void PlayerInput::MouseReleased(const InputType& inputType, std::shared_ptr<Particle>& particle) {
    if (particle == nullptr)
        return;

    if (inputType == Move) {
        return;
    } else if (inputType == Impulse) {
        particle->AddImpulse(mousePosition - originalPosition);
        trajectoryParticles.clear();
    }
}

void PlayerInput::MouseClicked(const InputType& inputType, std::shared_ptr<Particle>& particle) {
    if (particle == nullptr)
        return;

    if (inputType == Impulse) {
        if (particle->Type == Particle::Circle) {
            originalPosition = std::dynamic_pointer_cast<Circle>(particle)->Position;
        }
    }
}

void PlayerInput::CalculateTrajectory() {
    if (selectedParticle == nullptr)
        return;
    if (selectedParticle->Type != Particle::Circle)
        return;
    trajectoryParticles.clear();

    int steps = 4;
    float previewTime = 1.0f;
    std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(selectedParticle);

    for (int i = 1; i < steps + 1; ++i) {
        float timeStep = previewTime / steps * i;
        float powerTo2 = glm::pow(timeStep, 2);
        glm::vec2 velocity =
            circle->Velocity +
            (mousePosition - circle->Position) * circle->InverseMass;
        glm::vec2 acceleration = circle->TotalForce * circle->InverseMass;
        glm::vec2 pos = circle->Position + velocity * timeStep +
                        0.5f * acceleration * powerTo2;
        Circle trajectory = Circle::Circle(pos, glm::vec2(), circle->Radius);
        trajectory.Colour = Colors::gray;
        trajectoryParticles.push_back(
            std::make_shared<Circle>(trajectory));
    }
}