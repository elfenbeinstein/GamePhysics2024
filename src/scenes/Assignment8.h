#pragma once

#include <vector>
#include "core/Simple2DScene.h"
#include "geometry/Line.h"
#include "geometry/Particle.h"
#include "geometry/World.h"
#include "geometry/PlayerInput.h"
#include <memory>

class Assignment8 : public Simple2DScene {
public:
    Assignment8();
    ~Assignment8();

    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void DrawGUI() override;

    virtual const char* GetName() override { return "Assignment8"; };

private:
    std::vector<std::shared_ptr<Particle>> particles;
    World world;
    PlayerInput playerInput;
    std::shared_ptr<Circle> playerBall;
    std::shared_ptr<Circle> blackBall;
    PlayerInput::InputType inputType;
    int ballsCollected;
    int winCount;
    void ResetAll();
};
