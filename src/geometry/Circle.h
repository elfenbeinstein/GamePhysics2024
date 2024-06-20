#pragma once
#include "core/Simple2DScene.h"
#include "geometry/Particle.h"

class Circle : public Particle {
public:
    Circle();
    Circle(glm::vec2 pos, glm::vec2 vel, float rad);
    Circle(glm::vec2 pos, glm::vec2 vel, float rad, float mass);
    Circle(glm::vec2 pos, glm::vec2 vel, float rad, float mass, bool removeAfterCollision, bool removeOtherAfterCollision, bool canBeRemoved, bool canBeMovedByMouse, bool canAddImpulseByMouse);
    Circle(glm::vec2 pos, glm::vec2 vel, float rad, float mass, float damping, float coefficientOfRestitution,  bool removeAfterCollision, bool removeOtherAfterCollision, bool canBeRemoved, bool canBeMovedByMouse, bool canAddImpulseByMouse);
    Circle(glm::vec2 pos, glm::vec2 vel, float rad, float mass, float damping, float coefficientOfRestitution,  bool removeAfterCollision, bool removeOtherAfterCollision, bool canBeRemoved, bool canBeMovedByMouse, bool canAddImpulseByMouse, Color colour);

    glm::vec2 Acceleration;
    float Radius;
    float ColorCountdown;

    void Draw() override;
    void Update(float deltaTime, glm::vec2 gravity) override;
    void OnCollision() override;
    void Move(glm::vec2 previousMousePosition, glm::vec2 currentMousePosition) override;

    bool IsMouseOverParticle(glm::vec2 mousePosition) override;
};
