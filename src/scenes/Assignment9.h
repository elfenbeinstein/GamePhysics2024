#pragma once

#include <vector>
#include "core/Simple2DScene.h"
#include "geometry/Particle.h"
#include "geometry/World.h"
#include "geometry/PlayerInput.h"
#include "geometry/RigidBody.h"

class Assignment9 : public Simple2DScene {
public:
    Assignment9();
    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void DrawGUI() override;

    virtual const char* GetName() override { return "Assignment9"; };

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
