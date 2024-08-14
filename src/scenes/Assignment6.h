#pragma once

#include <vector>
#include "core/Simple2DScene.h"
#include "physics/Circle.h"
#include "physics/AxisAlignedBox.h"
#include "physics/Line.h"
#include "physics/Particle.h"
#include "physics/World.h"
#include "physics/PlayerInput.h"
#include <memory>

class Assignment6 : public Simple2DScene {
public:
    Assignment6();
    ~Assignment6();

    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void DrawGUI() override;

    virtual const char* GetName() override { return "Assignment6"; };

    void OnCollision(std::shared_ptr<Particle>& particle1, std::shared_ptr<Particle>& particle2);

private:
    std::vector<std::shared_ptr<Particle>> particles;
    std::shared_ptr<AxisAlignedBox> playerPaddle;
    World world;
    PlayerInput playerInput;

    void SetUpGame();
    void AddNewBall();

    float left;
    float right;
    float bottom;
    float top;

    float paddleWidth;
    float paddleHeight;
    float paddleY;

    float ballRadius;
    int playerLives;
    glm::vec2 startVelocity;
    std::shared_ptr<Circle> newBall;
    float paddleImpulse;
};
