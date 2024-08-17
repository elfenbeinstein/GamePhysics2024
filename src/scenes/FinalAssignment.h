#pragma once

#include <vector>
#include "core/Simple2DScene.h"
#include "physics/Particle.h"
#include "physics/World.h"
#include "physics/PlayerInput.h"
#include "physics/RigidBody.h"

class FinalAssignment : public Simple2DScene {
public:
    FinalAssignment();
    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void DrawGUI() override;

    virtual const char* GetName() override { return "FinalAssignment"; };

private:
    std::vector<std::shared_ptr<Particle>> particles;
    std::shared_ptr<Rectangle> magnet;
    World world;
    PlayerInput playerInput;
    bool useGravity;
    float damping;
    float coefficientOfRestitution;
    float angularDamp;
    int playerLives;
    glm::vec2* pointsA;
    glm::vec2* pointsB;
    glm::vec2 polygonAPos;
    glm::vec2 polygonBPos;
    std::vector<glm::vec2> polygonAPoints;
    std::vector<glm::vec2> polygonBPoints;
    bool gameWon;
    float time;
    int amountToWin;
    void SetUp();
    glm::vec2 CalculateMagneticForce(const std::shared_ptr<RigidBody>& magnet,
                                     const std::shared_ptr<RigidBody>& metal);
};
