#include "Assignment7.h"

#include "imgui.h"

Assignment7::Assignment7()
    : world(World(glm::vec2(), true, true)),
      inputType(PlayerInput::Impulse),
      randomGenerator(RandomParticleGenerator()),
      particles() {
    playerInput = PlayerInput::PlayerInput(PlayerInput::Impulse);
    ResetAll();
}

Assignment7::~Assignment7() {}

void Assignment7::OnEnable() {}

void Assignment7::OnDisable() {}

void Assignment7::Update(float deltaTime) {

    std::vector<std::shared_ptr<Particle>> particlesToRemove;
    playerInput.Update(particles);
    world.Update(particles, deltaTime, particlesToRemove, playerInput.selectedParticle);
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

void Assignment7::Draw() {
    for (auto& particle : particles) {
        particle->Draw();
    }
    playerInput.Draw();
}

void Assignment7::DrawGUI() {
    ImGui::Begin("Inspector");
    ImGui::Text("Choose Mouse Input Mode");
    static int current_item = static_cast<int>(playerInput.LeftMouseInputType);
    char* InputTypeNames[] = {"None", "Impulse", "Move"};
    if (ImGui::BeginCombo("Input Type", InputTypeNames[current_item])) {
        for (int n = 0; n < IM_ARRAYSIZE(InputTypeNames); n++) {
            bool is_selected = (current_item == n);
            if (ImGui::Selectable(InputTypeNames[n], is_selected)) {
                current_item = n;
                playerInput.LeftMouseInputType = static_cast<PlayerInput::InputType>(n); 
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Text("Adjust Values of first Circle");
    ImGui::DragFloat("Damping", &originalCircle->Damping, 0.1f);
    if (originalCircle->Damping < 0.0f)
        originalCircle->Damping = 0.0f;
    ImGui::DragFloat("CoR", &originalCircle->CoefficientOfRestitution, 0.1f);
    if (originalCircle->CoefficientOfRestitution < 0.0f)
        originalCircle->CoefficientOfRestitution = 0.0f;
    if (originalCircle->CoefficientOfRestitution > 1.0f)
        originalCircle->CoefficientOfRestitution = 1.0f;
    ImGui::Checkbox("Apply Damping", &world.ApplyDrag);
    ImGui::Checkbox("Resolve Collisions Inelastic", &world.ResolveCollisionsInelastic);

    ImGui::Spacing();
    if (ImGui::Button("Add Random Circle")) {
        particles.push_back(std::make_shared<Circle>(randomGenerator.CreateRandomCircle(0.2f, 1.0f, -3.0f, 3.0f, 0.0f, 0.0f, 0.7f, 1.21f, 0.0f, 8.0f, 0.0f, 1.0f, false, false, false, true, true)));
    }
    ImGui::Spacing();
    if (ImGui::Button("Reset All"))
        ResetAll();
    ImGui::End();
}

void Assignment7::ResetAll() {
    particles.clear();

    float top(5.0f), bottom(-5.0f), right(4.0f), left(-4.0f);
    particles.push_back(std::make_shared<Line>(
        glm::vec2(right, bottom), glm::vec2(left, bottom), true, false));
    particles.push_back(std::make_shared<Line>(
        glm::vec2(left, top), glm::vec2(right, top), false, false));
    particles.push_back(std::make_shared<Line>(
        glm::vec2(left, bottom), glm::vec2(left, top), false, false));
    particles.push_back(std::make_shared<Line>(
        glm::vec2(right, bottom), glm::vec2(right, top), false, false));

    originalCircle = std::make_shared<Circle>(
        glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 0.5f, 1.0f, 0.0f, 1.0f, false,
        false, false, true, true);
    particles.push_back(originalCircle);
}
