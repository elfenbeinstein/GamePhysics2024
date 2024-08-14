#pragma once

#include <vector>
#include "core/Simple2DScene.h"
#include "physics/Particle.h"
#include "physics/World.h"
#include "physics/PlayerInput.h"
#include "physics/RigidBody.h"

class Assignment10 : public Simple2DScene {
public:
    Assignment10();
    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void DrawGUI() override;

    virtual const char* GetName() override { return "Assignment10"; };

private:
    std::vector<std::shared_ptr<Particle>> particles;
    World world;
    PlayerInput playerInput;
    bool useGravity;
    float damping;
    float coefficientOfRestitution;
    float angularVelocity;
    float angularDamp;
};
