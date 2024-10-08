#include "Assignment6.h"

#include "imgui.h"
#include "core/Draw.h"
#include "physics/Circle.h"
#include "core/Colors.h"
#include <string>

Assignment6::Assignment6()
    : playerPaddle(std::make_shared<AxisAlignedBox>(glm::vec2(-0.5f, -4.75f),
                                                    glm::vec2(0.5f, -4.5f),
                                                    glm::vec2{0.0f, 0.0f},
                                                    glm::vec2{0.0f, 0.0f},
                                                    1.0f,
                                                    1.0f,
                                                    0.0f,
                                                    1.0f,
                                                    false,
                                                    false,
                                                    false,
                                                    true,
                                                    false,
                                                    Colors::white)),
      world(World(glm::vec2())),
      top(5.0f),
      bottom(-5.0f),
      right(4.0f),
      left(-4.0f),
      paddleWidth(1.0f),
      paddleHeight(0.25f),
      paddleY(-4.75f),
      ballRadius(0.15f),
      startVelocity(glm::vec2(0.9, 3.5f)),
      newBall(nullptr),
      playerLives(3),
      paddleImpulse(3.0f) {
    playerInput = PlayerInput::PlayerInput(PlayerInput::Move, false);
    SetUpGame();
    world.SetCollisionCallback(std::bind(&Assignment6::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
}

Assignment6::~Assignment6() {}

void Assignment6::OnEnable() {}

void Assignment6::OnDisable() {}

void Assignment6::Update(float deltaTime) {
    std::vector<std::shared_ptr<Particle>> particlesToRemove;
    playerInput.Update(particles);

    world.Update(particles, deltaTime, particlesToRemove, playerInput.selectedParticle);

    // player paddle constraints
    if (playerPaddle->Position.x < left) {
        playerPaddle->Position.x = left;
        playerPaddle->TopRight.x = left + paddleWidth;
    } else if (playerPaddle->TopRight.x > right) {
        playerPaddle->TopRight.x = right;
        playerPaddle->Position.x = right - paddleWidth;
    }
    if (playerPaddle->Position.y < paddleY) {
        playerPaddle->Position.y = paddleY;
        playerPaddle->TopRight.y = paddleY + paddleHeight;
    } else if (playerPaddle->Position.y > paddleY) {
        playerPaddle->Position.y = paddleY;
        playerPaddle->TopRight.y = paddleY + paddleHeight;
    }

    //move new ball with paddle
    if (newBall != nullptr) {
        newBall->Position =
            glm::vec2(playerPaddle->Position.x + paddleWidth / 2.0f,
                      playerPaddle->TopRight.y + ballRadius);
    }

    // release ball when space bar is pressed
    if (ImGui::IsKeyPressed(ImGuiKey_Space) && newBall != nullptr) {
        particles.push_back(std::make_shared<Circle>(
            newBall->Position, newBall->Velocity, newBall->Radius,
            newBall->Mass, 0.0f, 1.0f, false, true, true, false, false,
            Colors::white, 0.0f, 0.0f, 0.0f));
        newBall = nullptr;
    }

    // if ball was removed, add a new ball
    for (auto& particle : particlesToRemove) {
        if (particle->Type != Particle::Circle)
            continue;
        playerLives--;
        if (playerLives < 0) {
            SetUpGame();
            return;
        }

        AddNewBall();
    }

    // remove all particles that need to be removed
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [&](const std::shared_ptr<Particle>& particle) {
                           return std::find(particlesToRemove.begin(),
                                            particlesToRemove.end(),
                                            particle) !=
                                  particlesToRemove.end();
                       }),
        particles.end());

    if (particles.size() == 6) {
        SetUpGame();
    }
}

void Assignment6::Draw() {
    for (auto& particle : particles) {
        particle->Draw();
    }
    if (newBall != nullptr)
        newBall->Draw();
}

void Assignment6::DrawGUI() {
    ImGui::Begin("Inspector");
    ImGui::Text("Press Space to Release the Ball");


    if (ImGui::Button("Restart Game")) {
        particles.clear();
        SetUpGame();
    }
    ImGui::DragFloat2("Start Velocity", &startVelocity[0], 0.1f);
    ImGui::DragFloat("Paddle Impulse on Ball", &paddleImpulse, 0.1f);
    ImGui::Text("X impulse on ball on collision with paddle");
    ImGui::Spacing();
    ImGui::Text(("You have " + std::to_string(playerLives) + " lives remaining").c_str());
    
    ImGui::End();
}

void Assignment6::SetUpGame() {
    particles.clear();
    
    // set up lines surrounding game
    particles.push_back(std::make_shared<Line>(glm::vec2(right, bottom), glm::vec2(left, bottom), true, false));
    particles.push_back(std::make_shared<Line>(glm::vec2(left, top), glm::vec2(right, top), false, false));
    particles.push_back(std::make_shared<Line>(glm::vec2(left, bottom), glm::vec2(left, top), false, false));
    particles.push_back(std::make_shared<Line>(glm::vec2(right, bottom), glm::vec2(right, top), false, false));

    // set up boxes
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(-2.75f, 4.25f), glm::vec2(-1.75f, 4.5f),
        glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f,
        true, false, true, false, false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(-1.25f, 4.25f), glm::vec2(-0.25f, 4.5f),
        glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f,
        true, false, true, false, false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(0.25f, 4.25f), glm::vec2(1.25f, 4.5f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(1.75f, 4.25f), glm::vec2(2.75f, 4.5f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));

    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(-3.5f, 3.5f), glm::vec2(-2.5f, 3.75f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(-2.0f, 3.5f), glm::vec2(-1.0f, 3.75f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(-0.5f, 3.5f), glm::vec2(0.5f, 3.75f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(1.0f, 3.5f), glm::vec2(2.0f, 3.75f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(2.5f, 3.5f), glm::vec2(3.5f, 3.75f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));

    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(-2.75f, 2.75f), glm::vec2(-1.75f, 3.0f),
        glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f,
        true, false, true, false, false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(-1.25f, 2.75f), glm::vec2(-0.25f, 3.0f),
        glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f,
        true, false, true, false, false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(0.25f, 2.75f), glm::vec2(1.25f, 3.0f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(1.75f, 2.75f), glm::vec2(2.75f, 3.0f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));

    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(-3.5f, 2.0f), glm::vec2(-2.5f, 2.25f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(-2.0f, 2.0f), glm::vec2(-1.0f, 2.25f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(-0.5f, 2.0f), glm::vec2(0.5f, 2.25f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(1.0f, 2.0f), glm::vec2(2.0f, 2.25f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(2.5f, 2.0f), glm::vec2(3.5f, 2.25f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));

    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(-2.75f, 1.25f), glm::vec2(-1.75f, 1.5f),
        glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f,
        true, false, true, false, false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(-1.25f, 1.25f), glm::vec2(-0.25f, 1.5f),
        glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f,
        true, false, true, false, false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(0.25f, 1.25f), glm::vec2(1.25f, 1.5f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));
    particles.push_back(std::make_shared<AxisAlignedBox>(
        glm::vec2(1.75f, 1.25f), glm::vec2(2.75f, 1.5f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, true, false, true, false,
        false));

    playerPaddle = std::make_shared<AxisAlignedBox>(
        glm::vec2(-0.5f, -4.75f), glm::vec2(0.5f, -4.5f), glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, 1.0f, false, false, false,
        true, false, Colors::white);

    particles.push_back(playerPaddle);
    playerLives = 3;

    AddNewBall();
}

void Assignment6::AddNewBall() {
    newBall = std::make_shared<Circle>(
        glm::vec2(playerPaddle->Position.x + paddleWidth / 2.0f,
                  playerPaddle->TopRight.y + ballRadius),
       startVelocity, ballRadius, 1.0f, false, true, true, false, false);
}

// attempt to make this more interactive by adding an impulse based on where the ball hits the player's paddle
void Assignment6::OnCollision(std::shared_ptr<Particle>& particle1, std::shared_ptr<Particle>& particle2) {
    std::shared_ptr<Circle> circle;
    if (particle1 == playerPaddle && particle2->Type == Particle::Circle) {
        circle = std::dynamic_pointer_cast<Circle>(particle2);
    } else if (particle2 == playerPaddle && particle1->Type == Particle::Circle) {
        circle = std::dynamic_pointer_cast<Circle>(particle1);
    } else
        return;

    float factor = circle->Position.x - (playerPaddle->Position.x + paddleWidth / 2.0f);
    circle->AddImpulse(glm::vec2(factor * paddleImpulse, 0.0f));
}
