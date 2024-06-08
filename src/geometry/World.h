#pragma once

#include "geometry/Circle.h"
#include "geometry/Line.h"
#include "geometry/AxisAlignedBox.h"
#include <memory>
#include <vector>
#include <functional>
using CollisionCallback = std::function<void(std::shared_ptr<Particle>&, std::shared_ptr<Particle>&)>;

class World {
public:
    World();
    World(glm::vec2 gravity);

    void Update(std::vector<std::shared_ptr<Particle>>& particles, float deltaTime, std::vector<std::shared_ptr<Particle>>& particlesToRemove, std::vector<std::shared_ptr<Particle>>& selectedParticles);

    bool IsColliding(std::shared_ptr<Circle> circle1, std::shared_ptr<Circle> circle2, float& intersectionDepth, glm::vec2& collisionNormal);
    bool IsColliding(std::shared_ptr<Circle> circle, std::shared_ptr<Line> line, float& intersectionDepth, glm::vec2& collisionNormal);
    bool IsColliding(std::shared_ptr<Circle> circle, std::shared_ptr<AxisAlignedBox>box, float& intersectionDepth, glm::vec2& collisionNormal);

    void ResolveCollision(std::shared_ptr<Particle> particle1, std::shared_ptr<Particle> particle2, const float& intersectionDepth, const glm::vec2& collisionNormal);
    void SetCollisionCallback(CollisionCallback callback);

    glm::vec2 Gravity;

private:
    CollisionCallback collisionCallback;
};
