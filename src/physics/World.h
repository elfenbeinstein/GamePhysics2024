#pragma once

#include "physics/Circle.h"
#include "physics/Line.h"
#include "physics/AxisAlignedBox.h"
#include "physics/Rectangle.h"
#include "physics/CollisionManifold.h"
#include <memory>
#include <vector>
#include <functional>
#include "core/PhysicsUtils.h"

using CollisionCallback = std::function<void(std::shared_ptr<Particle>&, std::shared_ptr<Particle>&)>;

class World {
public:
    World();
    World(glm::vec2 gravity);
    World(glm::vec2 gravity, bool applyDrag, bool resolveCollisionsInelastic, bool resolveCollisionsWithImpulsiveTorque);

    void Update(std::vector<std::shared_ptr<Particle>>& particles, float deltaTime, std::vector<std::shared_ptr<Particle>>& particlesToRemove, std::shared_ptr<Particle>& selectedParticle);

    bool IsColliding(std::shared_ptr<Circle> circle1, std::shared_ptr<Circle> circle2, CollisionManifold& collisionManifold);
    bool IsColliding(std::shared_ptr<Circle> circle, std::shared_ptr<Line> line, CollisionManifold& collisionManifold);
    bool IsColliding(std::shared_ptr<Circle> circle, std::shared_ptr<AxisAlignedBox> box, CollisionManifold& collisionManifold);
    bool IsColliding(std::shared_ptr<Rectangle> rectA, std::shared_ptr<Rectangle> rectB, CollisionManifold& collisionManifold);
    bool IsColliding(std::shared_ptr<Rectangle> rectangle, std::shared_ptr<Circle> circle, CollisionManifold& collisionManifold);
    bool IsColliding(std::shared_ptr<Rectangle> rectangle, std::shared_ptr<Line> line, CollisionManifold& collisionManifold);
    bool IsColliding(std::shared_ptr<Rectangle> rectangle, std::shared_ptr<AxisAlignedBox> box, CollisionManifold& collisionManifold);

    void ResolveCollision(std::shared_ptr<Particle> particle1, std::shared_ptr<Particle> particle2, CollisionManifold& collisionManifold);
    void ResolveCollisionWithImpulsiveTorque(std::shared_ptr<Particle> particleA, std::shared_ptr<Particle> particleB, CollisionManifold& collisionManifold);
    float GetImpulseMagnitude(const glm::vec2& rA, const glm::vec2& rB, const float& coefficientOfRestitution, const float& separatingVelocity, 
        const float& inverseMassA, const float& inverseMassB, const float& inverseMomentOfInertiaA, const float& inverseMomentOfInertiaB, 
        CollisionManifold& collisionManifold);
    void SetCollisionCallback(CollisionCallback callback);

    glm::vec2 Gravity;
    bool ApplyDrag;
    bool ResolveCollisionsInelastic;
    bool ResolveCollisionsWithImpulsiveTorque;

private:
    CollisionCallback collisionCallback;
};
