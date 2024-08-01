#include "Assignment9.h"

#include "imgui.h"

Assignment9::Assignment9()
    : playerInput(),
      world(glm::vec2{0.0f, 0.0f}, true, true),
      particles(),
      useGravity(false),
      damping(0.5f),
      coefficientOfRestitution(0.5f),
      angularVelocity(0.0f),
      angularDamp(0.5f) {
    particles.push_back(std::make_shared<Line>(
        glm::vec2(10.0f, 0.0f), glm::vec2(-10.0f, 0.0f), true, false));
}

void Assignment9::OnEnable() {}

void Assignment9::OnDisable() {}

void Assignment9::Update(float deltaTime) {
    std::vector<std::shared_ptr<Particle>> particlesToRemove;
    playerInput.Update(particles);
    world.Update(particles, deltaTime, particlesToRemove,
                 playerInput.selectedParticle);

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [&](const std::shared_ptr<Particle>& particle) {
                           return std::find(particlesToRemove.begin(),
                                            particlesToRemove.end(),
                                            particle) !=
                                  particlesToRemove.end();
                       }),
        particles.end());

    for (auto& particle : particles) {
        particle->CoefficientOfRestitution = coefficientOfRestitution;
        particle->Damping = damping;
        if (particle->Type == Particle::Rectangle ||
            particle->Type == Particle::Circle) {
            std::shared_ptr<RigidBody> rb = std::dynamic_pointer_cast<RigidBody>(particle);
            if (rb == nullptr)
                continue;
            rb->AngularVelocity = angularVelocity;
            rb->AngularDamp = angularDamp;
        }
    }
}

void Assignment9::Draw() {
    for (auto& particle : particles) {
        particle->Draw();
    }
    playerInput.Draw();
}

void Assignment9::DrawGUI() {
    ImGui::Begin("Inspector");
    ImGui::Text("Choose Mouse Input Mode");
    static int current_item = static_cast<int>(playerInput.LeftMouseInputType);
    char* InputTypeNames[] = {"None",     "Impulse",    "Move",
                              "DrawRect", "DrawCircle", "DrawLine"};
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

    ImGui::Text("Global Values:");
    ImGui::Checkbox("Use Gravity", &useGravity);
    if (useGravity)
        world.Gravity = glm::vec2{0, -9.81f};
    else
        world.Gravity = glm::vec2{0.0f, 0.0f};
    ImGui::DragFloat("Damping", &damping, 0.01f);
    ImGui::DragFloat("CoR", &coefficientOfRestitution, 0.01f);
    ImGui::DragFloat("Angular Damp", &angularDamp, 0.01f);
    ImGui::DragFloat("Angular Vel", &angularVelocity, 0.01f);

    ImGui::Spacing();
    if (ImGui::Button("Clear All")) {
        particles.clear();
    }

    ImGui::End();
}
