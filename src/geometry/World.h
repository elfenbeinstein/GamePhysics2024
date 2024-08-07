#pragma once

#include "geometry/Circle.h"
#include "geometry/Line.h"
#include "geometry/AxisAlignedBox.h"
#include "geometry/Rectangle.h"
#include <memory>
#include <vector>
#include <functional>
#include "core/PhysicsUtils.h"

using CollisionCallback = std::function<void(std::shared_ptr<Particle>&, std::shared_ptr<Particle>&)>;

class World {
public:
    World();
    World(glm::vec2 gravity);
    World(glm::vec2 gravity, bool applyDrag, bool resolveCollisionsInelastic);

    void Update(std::vector<std::shared_ptr<Particle>>& particles, float deltaTime, std::vector<std::shared_ptr<Particle>>& particlesToRemove, std::shared_ptr<Particle>& selectedParticle);

    bool IsColliding(std::shared_ptr<Circle> circle1, std::shared_ptr<Circle> circle2, float& intersectionDepth, glm::vec2& collisionNormal);
    bool IsColliding(std::shared_ptr<Circle> circle, std::shared_ptr<Line> line, float& intersectionDepth, glm::vec2& collisionNormal);
    bool IsColliding(std::shared_ptr<Circle> circle, std::shared_ptr<AxisAlignedBox> box, float& intersectionDepth, glm::vec2& collisionNormal);
    bool IsColliding(std::shared_ptr<Rectangle> rectA, std::shared_ptr<Rectangle> rectB, float& intersectionDepth, glm::vec2& collisionNormal);
    bool IsColliding(std::shared_ptr<Rectangle> rectangle, std::shared_ptr<Circle> circle, float& intersectionDepth, glm::vec2& collisionNormal);
    bool IsColliding(std::shared_ptr<Rectangle> rectangle, std::shared_ptr<Line> line, float& intersectionDepth, glm::vec2& collisionNormal);
    bool IsColliding(std::shared_ptr<Rectangle> rectangle, std::shared_ptr<AxisAlignedBox> box, float& intersectionDepth, glm::vec2& collisionNormal);

    void ResolveCollision(std::shared_ptr<Particle> particle1, std::shared_ptr<Particle> particle2, const float& intersectionDepth, glm::vec2& collisionNormal);
    void SetCollisionCallback(CollisionCallback callback);

    glm::vec2 Gravity;
    bool ApplyDrag;
    bool ResolveCollisionsInelastic;

private:
    CollisionCallback collisionCallback;
};
