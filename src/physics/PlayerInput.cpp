#include "PlayerInput.h"
#include "core/Draw.h"

PlayerInput::PlayerInput()
    : LeftMouseInputType(None),
      leftMouseDown(false),
      mousePosition(),
      originalPosition(),
      PreviewTime(1.0f),
      PreviewSteps(4),
      selectedParticle(nullptr),
      originalColour(),
      ApplyImpulsiveTorque(false) {}

PlayerInput::PlayerInput(InputType leftMouseInput, bool applyImpulsiveTorque)
    : LeftMouseInputType(leftMouseInput),
      leftMouseDown(false),
      mousePosition(),
      originalPosition(),
      PreviewTime(1.0f),
      PreviewSteps(4),
      selectedParticle(nullptr),
      originalColour(),
      ApplyImpulsiveTorque(applyImpulsiveTorque) {}

void PlayerInput::Draw() {
    if (previewParticles.size() == 0)
        return;
    for (auto& particle : previewParticles) {
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
            if (LeftMouseInputType == Impulse || LeftMouseInputType == Move ||
                LeftMouseInputType == RemoveParticle) {
                for (std::size_t i = 0; i < particles.size(); ++i) {
                    if (!particles[i]->IsMouseOverParticle(mousePosition))
                        continue;
                    if ((LeftMouseInputType == Impulse) &&
                        !particles[i]->CanAddImpulseByMouse)
                        continue;
                    if (LeftMouseInputType == Move &&
                        !particles[i]->CanBeMovedByMouse)
                        continue;
                    selectedParticle = particles[i];

                    leftMouseDown = true;
                    if (selectedParticle != nullptr) {
                        originalColour = selectedParticle->Colour;
                        selectedParticle->Colour = Colors::cyan;
                    }
                    break;
                }
            }
            
            MouseClicked(LeftMouseInputType, selectedParticle);
        }
        if (Input::IsMouseReleased(0) && leftMouseDown) {
            MouseReleased(LeftMouseInputType, selectedParticle);
            
            if (LeftMouseInputType == DrawCircle) {
                previewParticles.clear();
                float radius =
                    glm::abs(glm::distance(originalPosition, mousePosition));
                Circle circle =
                    Circle(originalPosition, glm::vec2{0.0f, 0.0f}, radius,
                           1.0f, 0.5f, 0.5f, false, false, false, true, true,
                           Colors::white, 0.0f, 0.2f, 0.0f);
                if (radius > 0.001f)
                    particles.push_back(std::make_shared<Circle>(circle));
            } else if (LeftMouseInputType == DrawRect) {
                previewParticles.clear();
                float length = glm::distance(mousePosition, originalPosition);
                if (length > 0.1f) {
                    glm::vec2 halfExtends = {0.0f, 0.0f};
                    halfExtends.x =
                        glm::abs(originalPosition.x - mousePosition.x) / 2.0f;
                    halfExtends.y =
                        glm::abs(originalPosition.y - mousePosition.y) / 2.0f;
                    glm::vec2 pos = (mousePosition + originalPosition) / 2.0f;
                    Rectangle rect =
                        Rectangle(halfExtends, pos, glm::vec2{0.0f, 0.0f}, 1.0f,
                                  0.5f, 0.5f, 0.0f, 0.0f, 0.0f, false, false,
                                  false, true, true, Colors::white);
                    if (halfExtends.x > 0.001f && halfExtends.y > 0.001f)
                        particles.push_back(std::make_shared<Rectangle>(rect));
                }
            } else if (LeftMouseInputType == DrawLine) {
                previewParticles.clear();
                float length = glm::distance(mousePosition, originalPosition);
                if (length > 0.1f) {
                    Line line =
                        Line(mousePosition, originalPosition, 0.0f, 0.0f, 1.0f,
                             {0.0f, 0.0f}, {0.0f, 0.0f}, mousePosition, false,
                             false, true, true, false, Colors::blue);
                    particles.push_back(std::make_shared<Line>(line));
                }
            } else if (LeftMouseInputType == DrawAABB) {
                previewParticles.clear();
                float length = glm::distance(mousePosition, originalPosition);
                if (length > 0.1f) {
                    float minX = std::min(mousePosition.x, originalPosition.x);
                    float minY = std::min(mousePosition.y, originalPosition.y);
                    glm::vec2 bottomLeft(minX, minY);
                    float maxX = std::max(mousePosition.x, originalPosition.x);
                    float maxY = std::max(mousePosition.y, originalPosition.y);
                    glm::vec2 topRight(maxX, maxY);
                    AxisAlignedBox box = AxisAlignedBox(
                        bottomLeft, topRight, {0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f,
                        0.0f, 0.0f, 1.0f, false, false, true, true, false,
                        Colors::white);
                    particles.push_back(std::make_shared<AxisAlignedBox>(box));
                }
            } else if (LeftMouseInputType == RemoveParticle) {
                auto it = std::remove(particles.begin(), particles.end(),
                                      selectedParticle);
                particles.erase(it, particles.end());
            }

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
    if (inputType == Move) {
        if (particle == nullptr)
            return;
        particle->Move(prevMousePosition, mousePosition);
    } else if (inputType == Impulse) {
        if (particle == nullptr)
            return;
        CalculateTrajectory();
    } else if (inputType == DrawCircle) {
        previewParticles.clear();
        float radius = glm::abs(glm::distance(originalPosition, mousePosition));
        Circle circle = Circle(originalPosition, glm::vec2{0.0f, 0.0f}, radius);
        circle.Colour = Colors::gray;
        previewParticles.push_back(
            std::make_shared<Circle>(circle));
    } else if (inputType == DrawRect || inputType == DrawAABB) {
        previewParticles.clear();
        glm::vec2 halfExtends = {0.0f, 0.0f};
        halfExtends.x = glm::abs(originalPosition.x - mousePosition.x) / 2.0f;
        halfExtends.y = glm::abs(originalPosition.y - mousePosition.y) / 2.0f;
        glm::vec2 pos = (mousePosition + originalPosition) / 2.0f;
        Rectangle rect =
            Rectangle(halfExtends, pos, glm::vec2{0.0f, 0.0f},
                      1.0f, 0.5f, 0.5f, 0.0f, 0.8f, 0.2f, false, false, false,
                      true, true, Colors::gray);
        previewParticles.push_back(std::make_shared<Rectangle>(rect));
    } else if (inputType == DrawLine) {
        previewParticles.clear();
        previewParticles.push_back(
            std::make_shared<Line>(originalPosition, mousePosition, false, false));
    }
}

void PlayerInput::MouseReleased(const InputType& inputType, std::shared_ptr<Particle>& particle) {
    leftMouseDown = false;
    
    if (inputType == Move) {
        return;
    } else if (inputType == Impulse) {
        if (particle == nullptr)
            return;
        if (ApplyImpulsiveTorque) {
            std::shared_ptr<RigidBody> rigidBody =
                std::dynamic_pointer_cast<RigidBody>(particle);
            rigidBody->AddImpulseWithTorque(mousePosition - originalPosition,
                                            mousePosition);
        } else {
            particle->AddImpulse(mousePosition - originalPosition);
        }
        previewParticles.clear();
    } 
}

void PlayerInput::MouseClicked(const InputType& inputType, std::shared_ptr<Particle>& particle) {
    if (inputType == Impulse) {
        if (particle == nullptr)
            return;
        
        if (ApplyImpulsiveTorque) {
            originalPosition = mousePosition;
        } else {
            if (particle->Type == Particle::Circle) {
                originalPosition =
                    std::dynamic_pointer_cast<Circle>(particle)->Position;
            } else if (particle->Type == Particle::Rectangle) {
                originalPosition =
                    std::dynamic_pointer_cast<Rectangle>(particle)->Position;
            }
        }
    } else if (inputType == Move) {
    } else {
        originalPosition = mousePosition;
        leftMouseDown = true;
    }
}

void PlayerInput::CalculateTrajectory() {
    if (selectedParticle == nullptr)
        return;
    previewParticles.clear();

    if (selectedParticle->Type == Particle::Circle) {
        std::shared_ptr<Circle> circle =
            std::dynamic_pointer_cast<Circle>(selectedParticle);

        for (int i = 1; i < PreviewSteps + 1; ++i) {
            float timeStep = PreviewTime / PreviewSteps * i;
            float powerTo2 = glm::pow(timeStep, 2);
            glm::vec2 velocity =
                circle->Velocity +
                (mousePosition - circle->Position) * circle->InverseMass;
            glm::vec2 acceleration = circle->TotalForce * circle->InverseMass;
            glm::vec2 pos = circle->Position + velocity * timeStep +
                            0.5f * acceleration * powerTo2;
            Circle trajectory =
                Circle::Circle(pos, glm::vec2(), circle->Radius);
            trajectory.Colour = Colors::gray;
            previewParticles.push_back(std::make_shared<Circle>(trajectory));
        }
    } else if (selectedParticle->Type == Particle::Rectangle) {
        std::shared_ptr<Rectangle> rectangle =
            std::dynamic_pointer_cast<Rectangle>(selectedParticle);

        for (int i = 1; i < PreviewSteps + 1; ++i) {
            float timeStep = PreviewTime / PreviewSteps * i;
            float powerTo2 = glm::pow(timeStep, 2);
            glm::vec2 velocity =
                rectangle->Velocity +
                (mousePosition - rectangle->Position) * rectangle->InverseMass;
            glm::vec2 acceleration =
                rectangle->TotalForce * rectangle->InverseMass;
            glm::vec2 pos = rectangle->Position + velocity * timeStep +
                            0.5f * acceleration * powerTo2;
            Rectangle trajectory = Rectangle::Rectangle(
                rectangle->HalfExtends, pos, glm::vec2(), 0.0f, 0.0f, 0.0f,
                rectangle->Angle, 0.0f, 0.0f, false, false, false, false, false,
                Colors::gray);
            previewParticles.push_back(std::make_shared<Rectangle>(trajectory));
        }
    }
}