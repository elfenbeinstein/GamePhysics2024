#pragma once

#include <vector>
#include "core/Simple2DScene.h"
#include "physics/Line.h"
#include "physics/Particle.h"
#include "physics/World.h"
#include "physics/PlayerInput.h"
#include "physics/RandomParticleGenerator.h"
#include <memory>

class Assignment7 : public Simple2DScene {
public:
    Assignment7();
    ~Assignment7();

    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void DrawGUI() override;

    virtual const char* GetName() override { return "Assignment7"; };

private:
    std::vector<std::shared_ptr<Particle>> particles;
    World world;
    RandomParticleGenerator randomGenerator;
    PlayerInput playerInput;
    std::shared_ptr<Circle> originalCircle;
    PlayerInput::InputType inputType;
    void ResetAll();
};
