#include "PlayerInput.h"
#include "core/Draw.h"

PlayerInput::PlayerInput()
    : LeftMouseInputType(None),
      leftMouseDown(false),
      mousePosition(),
      originalPosition(),
      selectedParticles(3, nullptr) {}

PlayerInput::PlayerInput(InputType leftMouseInput)
    : LeftMouseInputType(leftMouseInput),
      leftMouseDown(false),
      mousePosition(),
      originalPosition(),
      selectedParticles(3, nullptr) {}

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
                selectedParticles[0] = particles[i];
                MouseClicked(LeftMouseInputType, selectedParticles[0]);
                break;
            }
        }
        if (Input::IsMouseReleased(0)) {
            MouseReleased(LeftMouseInputType, selectedParticles[0]);
            selectedParticles[0] = nullptr;
        }
        if (leftMouseDown) {
            MouseDown(LeftMouseInputType, selectedParticles[0], previousMousePosition);
        }
    }
}

void PlayerInput::MouseDown(const InputType& inputType, std::shared_ptr<Particle>& particle, const glm::vec2 prevMousePosition) {
    if (particle == nullptr)
        return;

    if (inputType == Move) {
        particle->Move(prevMousePosition, mousePosition);
    } else if (inputType == Impulse) {
        //calculate trajectory
    }
}

void PlayerInput::MouseReleased(const InputType& inputType, std::shared_ptr<Particle>& particle) {
    if (particle == nullptr)
        return;

    if (inputType == Move) {
        return;
    } else if (inputType == Impulse) {
        particle->AddImpulse(mousePosition - originalPosition);
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