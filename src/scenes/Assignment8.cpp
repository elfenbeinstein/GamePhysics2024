#include "Assignment8.h"

#include "imgui.h"
#include "core/Draw.h"

Assignment8::Assignment8()
    : world(World(glm::vec2(), true, true)),
      inputType(PlayerInput::Impulse),
      ballsCollected(0),
      winCount(0),
      particles() {
    playerInput = PlayerInput::PlayerInput(PlayerInput::Impulse);
    playerInput.PreviewSteps = 6;
    playerInput.PreviewTime = 0.5f;
    ResetAll();
}

Assignment8::~Assignment8() {}

void Assignment8::OnEnable() {}

void Assignment8::OnDisable() {}

void Assignment8::Update(float deltaTime) {
    std::vector<std::shared_ptr<Particle>> particlesToRemove;
    playerInput.Update(particles);
    world.Update(particles, deltaTime, particlesToRemove,
                 playerInput.selectedParticle);

    for (auto& particle : particlesToRemove) {
        if (particle->Type != Particle::Circle)
            continue;

        if (particle == playerBall) {
            playerBall = std::make_shared<Circle>(
                glm::vec2(0.0f, -2.0f), glm::vec2(0.0f, 0.0f), 0.2f, 1.0f,
                1.0f, 0.6f, false, false, true, true, true);
            particles.push_back(playerBall);
        } else if (particle == blackBall) {
            if (ballsCollected < 14)
                ResetAll();
            else {
                winCount++;
            }
        } else {
            ballsCollected++;
        }
    }

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [&](const std::shared_ptr<Particle>& particle) {
                           return std::find(particlesToRemove.begin(),
                                            particlesToRemove.end(),
                                            particle) !=
                                  particlesToRemove.end();
                       }),
        particles.end());
}

void Assignment8::Draw() {
    for (auto& particle : particles) {
        if (particle == playerBall) {
            Draw::SetColor(Colors::white);
            Draw::Circle(playerBall->Position, playerBall->Radius, true);
            Draw::SetColor(Colors::white);
        } else if (particle == blackBall) {
            Draw::SetColor(Colors::orange);
            Draw::Circle(blackBall->Position, blackBall->Radius, true);
            Draw::SetColor(Colors::white);  
        } else {
            particle->Draw();
        }

    }
    playerInput.Draw();
}

void Assignment8::DrawGUI() {
    ImGui::Begin("Inspector");
    ImGui::Text("Choose Mouse Input Mode");
    static int current_item = static_cast<int>(playerInput.LeftMouseInputType);
    char* InputTypeNames[] = {"None", "Impulse", "Move"};
    if (ImGui::BeginCombo("Input Type", InputTypeNames[current_item])) {
        for (int n = 0; n < IM_ARRAYSIZE(InputTypeNames); n++) {
            bool is_selected = (current_item == n);
            if (ImGui::Selectable(InputTypeNames[n], is_selected)) {
                current_item = n;
                playerInput.LeftMouseInputType =
                    static_cast<PlayerInput::InputType>(n);
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Text("Adjust Values of Balls");
    ImGui::DragFloat("Damping", &playerBall->Damping, 0.01f);
    if (playerBall->Damping < 0.0f)
        playerBall->Damping = 0.0f;
    ImGui::DragFloat("CoR", &playerBall->CoefficientOfRestitution, 0.01f);
    if (playerBall->CoefficientOfRestitution < 0.0f)
        playerBall->CoefficientOfRestitution = 0.0f;
    if (playerBall->CoefficientOfRestitution > 1.0f)
        playerBall->CoefficientOfRestitution = 1.0f;

    ImGui::Spacing();
    if (ImGui::Button("Reset All"))
        ResetAll();
    ImGui::Text("Sink the orange one last!");

    if (winCount > 0)
        ImGui::Text("YOU WON!");
    ImGui::End();

    for (auto& particle : particles) {
        if (particle->Type == Particle::Circle && particle->Mass > 0.0001f) {
            particle->Damping = playerBall->Damping;
            particle->CoefficientOfRestitution = playerBall->CoefficientOfRestitution;
        }
    }
}

void Assignment8::ResetAll() {
    particles.clear();
    ballsCollected = 0;
    winCount = 0;

    float top(5.0f), bottom(-5.0f), right(4.0f), left(-4.0f), radius(0.25f),
        ballMass(0.5f);

    // lines
    particles.push_back(std::make_shared<Line>(glm::vec2(right - radius, bottom),
                               glm::vec2(0.0f + radius, bottom), false, false));
    particles.push_back(std::make_shared<Line>(glm::vec2(0.0f - radius, bottom),
                               glm::vec2(left + radius, bottom), false, false));
    particles.push_back(std::make_shared<Line>(glm::vec2(right - radius, top),
                               glm::vec2(0.0f + radius, top), false, false));
    particles.push_back(std::make_shared<Line>(glm::vec2(0.0f - radius, top),
                               glm::vec2(left + radius, top), false, false));
    particles.push_back(std::make_shared<Line>(glm::vec2(right, top - radius),
                               glm::vec2(right, 0.0f + radius), false, false));
    particles.push_back(std::make_shared<Line>(glm::vec2(right, 0.0f - radius),
                               glm::vec2(right, bottom + radius), false, false));
    particles.push_back(std::make_shared<Line>(glm::vec2(left, top - radius),
                               glm::vec2(left, 0.0f + radius), false, false));
    particles.push_back(std::make_shared<Line>(glm::vec2(left, 0.0f - radius),
                               glm::vec2(left, bottom + radius), false, false));

    // holes
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(0.0f, bottom), glm::vec2(), radius, 0.0f, 0.0f, 1.0f, false,
        true, false, false, false, Colors::blue));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(right, bottom), glm::vec2(), radius, 0.0f, 0.0f, 1.0f, false,
        true, false, false, false, Colors::blue));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(left, bottom), glm::vec2(), radius, 0.0f, 0.0f, 1.0f, false,
        true, false, false, false, Colors::blue));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(right, 0.0f), glm::vec2(), radius, 0.0f, 0.0f, 1.0f, false,
        true, false, false, false, Colors::blue));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(left, 0.0f), glm::vec2(), radius, 0.0f, 0.0f, 1.0f, false,
        true, false, false, false, Colors::blue));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(0.0f, top), glm::vec2(), radius, 0.0f, 0.0f, 1.0f, false,
        true, false, false, false, Colors::blue));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(right, top), glm::vec2(), radius, 0.0f, 0.0f, 1.0f, false,
        true, false, false, false, Colors::blue));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(left, top), glm::vec2(), radius, 0.0f, 0.0f, 1.0f, false,
        true, false, false, false, Colors::blue));

    // balls
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(0.0f, 1.5f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(-0.25f, 1.9f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(0.25f, 1.9f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(-0.5f, 2.3f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(0.5f, 2.3f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(-0.75f, 2.7f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(-0.25f, 2.7f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(0.25f, 2.7f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(0.75f, 2.7f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(-1.0f, 3.1f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false, 
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(-0.5f, 3.1f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(0.f, 3.1f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(0.5f, 3.1f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));
    particles.push_back(std::make_shared<Circle>(
        glm::vec2(1.0f, 3.1f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::white));

    blackBall = std::make_shared<Circle>(
        glm::vec2(0.0f, 2.3f), glm::vec2(), 0.2f, ballMass, 1.0f, 0.6f, false,
        false, true, false, false, Colors::orange);
    particles.push_back(blackBall);

    playerBall = std::make_shared<Circle>(
        glm::vec2(0.0f, -2.0f), glm::vec2(0.0f, 0.0f), 0.2f, ballMass, 0.61f, 0.6f,
        false, false, true, true, true);
    particles.push_back(playerBall);
}
