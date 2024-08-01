#include "World.h"

World::World()
    : Gravity({0, -9.81f}), ApplyDrag(false), ResolveCollisionsInelastic(false) {}

World::World(glm::vec2 gravity)
    : Gravity(gravity), ApplyDrag(false), ResolveCollisionsInelastic(false) {}

World::World(glm::vec2 gravity, bool applyDrag, bool resolveCollisionsInelastic)
    : Gravity(gravity),
      ApplyDrag(applyDrag),
      ResolveCollisionsInelastic(resolveCollisionsInelastic) {}

void World::SetCollisionCallback(CollisionCallback callback) {
    collisionCallback = callback;
}

void World::Update(std::vector<std::shared_ptr<Particle>>& particles, float deltaTime, std::vector<std::shared_ptr<Particle>>& particlesToRemove, std::shared_ptr<Particle>& selectedParticle) {
    for (auto& particle : particles) {
        if (particle == selectedParticle)
            continue;
        particle->Update(deltaTime, Gravity);
        if (ApplyDrag) {
            particle->Velocity *= 1.0f - particle->Damping * deltaTime;
        }
    }

    for (int i = 0; i < particles.size(); ++i) {
        bool isColliding = false;
        glm::vec2 normal;
        float intersectionDepth;

        for (int j = i + 1; j < particles.size(); ++j) {
            if (particles[i]->Type == Particle::Circle &&
                particles[j]->Type == Particle::Circle) {
                isColliding =
                    IsColliding(std::dynamic_pointer_cast<Circle>(particles[i]),
                                std::dynamic_pointer_cast<Circle>(particles[j]),
                                intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::Circle &&
                       particles[j]->Type == Particle::Line) {
                isColliding =
                    IsColliding(std::dynamic_pointer_cast<Circle>(particles[i]),
                                std::dynamic_pointer_cast<Line>(particles[j]),
                                intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::Circle &&
                       particles[j]->Type == Particle::AABB) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Circle>(particles[i]),
                    std::dynamic_pointer_cast<AxisAlignedBox>(particles[j]),
                    intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::Circle &&
                       particles[j]->Type == Particle::Rectangle) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[j]),
                    std::dynamic_pointer_cast<Circle>(particles[i]),
                    intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::Line &&
                       particles[j]->Type == Particle::Circle) {
                isColliding =
                    IsColliding(std::dynamic_pointer_cast<Circle>(particles[j]),
                                std::dynamic_pointer_cast<Line>(particles[i]),
                                intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::Line &&
                       particles[j]->Type == Particle::Rectangle) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[j]),
                    std::dynamic_pointer_cast<Line>(particles[i]),
                    intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::AABB &&
                       particles[j]->Type == Particle::Circle) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Circle>(particles[j]),
                    std::dynamic_pointer_cast<AxisAlignedBox>(particles[i]),
                    intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::AABB &&
                       particles[j]->Type == Particle::Rectangle) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[j]),
                    std::dynamic_pointer_cast<AxisAlignedBox>(particles[i]),
                    intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::Rectangle &&
                       particles[j]->Type == Particle::Rectangle) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[i]),
                    std::dynamic_pointer_cast<Rectangle>(particles[j]),
                    intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::Rectangle &&
                       particles[j]->Type == Particle::Circle) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[i]),
                    std::dynamic_pointer_cast<Circle>(particles[j]),
                    intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::Rectangle &&
                       particles[j]->Type == Particle::Line) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[i]),
                    std::dynamic_pointer_cast<Line>(particles[j]),
                    intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::Rectangle &&
                       particles[j]->Type == Particle::AABB) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[i]),
                    std::dynamic_pointer_cast<AxisAlignedBox>(particles[j]),
                    intersectionDepth, normal);
            } 

            if (isColliding) {
                ResolveCollision(particles[i], particles[j], intersectionDepth, normal);

                if (collisionCallback) {
                    collisionCallback(particles[i], particles[j]);
                }

                if ((particles[i]->RemoveThisAfterCollision ||
                     particles[j]->RemoveOtherParticleAfterCollision) &&
                    particles[i]->CanBeRemovedOnCollision)
                    particlesToRemove.push_back(particles[i]);
                if ((particles[i]->RemoveOtherParticleAfterCollision ||
                     particles[j]->RemoveThisAfterCollision) &&
                    particles[j]->CanBeRemovedOnCollision)
                    particlesToRemove.push_back(particles[j]);
            }
        }
    }
}

bool World::IsColliding(std::shared_ptr<Circle> circle1, std::shared_ptr<Circle> circle2, float& intersectionDepth, glm::vec2& collisionNormal) {
    float distance =
        std::sqrt(std::pow(circle2->Position.x - circle1->Position.x, 2) +
                  std::pow(circle2->Position.y - circle1->Position.y, 2));
    if (distance > circle1->Radius + circle2->Radius)
        return false;
    intersectionDepth = circle1->Radius + circle2->Radius - distance;
    collisionNormal = glm::normalize(glm::vec2(circle2->Position.x - circle1->Position.x,
                                               circle2->Position.y - circle1->Position.y));

    return true;
}

bool World::IsColliding(std::shared_ptr<Circle> circle, std::shared_ptr<Line> line, float& intersectionDepth, glm::vec2& collisionNormal) {
    float radiusSquared = std::pow(circle->Radius, 2);
    float distanceToStartSquared =
        std::pow(line->Position.x - circle->Position.x, 2) +
        std::pow(line->Position.y - circle->Position.y, 2);
    collisionNormal = line->Normal;


    if (distanceToStartSquared <= radiusSquared) {
        intersectionDepth = std::sqrt(radiusSquared - distanceToStartSquared);
        return true;
    }

    float distanceToEndSquared = std::pow(line->EndOffset.x - circle->Position.x, 2) +
                                 std::pow(line->EndOffset.y - circle->Position.y, 2);
    if (distanceToEndSquared <= radiusSquared) {
        intersectionDepth = std::sqrt(radiusSquared - distanceToEndSquared);
        return true;
    }

    float projectedDistance =
        glm::dot(circle->Position, collisionNormal) - line->ProjectionOntoNormal;
    if (glm::abs(projectedDistance) > circle->Radius)
        return false;

    float distanceClosestPointFromStart = glm::sqrt(
        distanceToStartSquared + glm::pow(glm::abs(projectedDistance), 2));
    float distanceClosestPointFromEnd = glm::sqrt(
        distanceToEndSquared + glm::pow(glm::abs(projectedDistance), 2));

    float lineLength = glm::distance(line->Position, line->EndOffset);

    if (distanceClosestPointFromStart > lineLength ||
        distanceClosestPointFromEnd > lineLength)
        return false;

    intersectionDepth = circle->Radius - projectedDistance;
    if (projectedDistance < 0) {
        intersectionDepth = circle->Radius + projectedDistance;
        collisionNormal = -collisionNormal;
    }
    return true;
}

bool World::IsColliding(std::shared_ptr<Circle> circle, std::shared_ptr<AxisAlignedBox>box, float& intersectionDepth, glm::vec2& collisionNormal) {
    glm::vec2 closestPoint = box->ClosestPoint(circle->Position);
    float distanceSquared = std::pow(circle->Position.x - closestPoint.x, 2) +
                            std::pow(circle->Position.y - closestPoint.y, 2);

    if (distanceSquared > std::pow(circle->Radius, 2))
        return false;

    intersectionDepth = circle->Radius - std::sqrt(distanceSquared);
    collisionNormal = glm::normalize(circle->Position - closestPoint);
    return true;
}

bool World::IsColliding(std::shared_ptr<Rectangle> rectA, std::shared_ptr<Rectangle> rectB, float& intersectionDepth, glm::vec2& collisionNormal) {
    return false;
}

bool World::IsColliding(std::shared_ptr<Rectangle> rectangle, std::shared_ptr<Line> line, float& intersectionDepth, glm::vec2& collisionNormal) {
    return false;
}

bool World::IsColliding(std::shared_ptr<Rectangle> rectangle, std::shared_ptr<AxisAlignedBox> box, float& intersectionDepth, glm::vec2& collisionNormal) {
    return false;
}

bool World::IsColliding(std::shared_ptr<Rectangle> rectangle, std::shared_ptr<Circle> circle, float& intersectionDepth, glm::vec2& collisionNormal) {
    return false;
}

void World::ResolveCollision(std::shared_ptr<Particle> particle1, std::shared_ptr<Particle> particle2, const float& intersectionDepth, glm::vec2& collisionNormal) {
    float totalMass = particle1->Mass + particle2->Mass;
    if (totalMass < 0.0001f)
        return;
    particle1->OnCollision();
    particle2->OnCollision();

    if (particle2->Type == Particle::Circle && particle2->Type != Particle::Circle) {
        collisionNormal = -collisionNormal;
    }

    if (particle2->Mass < 0.0001f) {
        particle1->Position -= collisionNormal * intersectionDepth;
    } else if (particle1->Mass < 0.0001f) {
        particle2->Position += collisionNormal * intersectionDepth;
    } else {
        float massRatio1 = particle1->Mass / totalMass;
        float massRatio2 = particle2->Mass / totalMass;

        particle1->Position -= collisionNormal * (intersectionDepth * massRatio2);
        particle2->Position += collisionNormal * (intersectionDepth * massRatio1);
    }


    if (ResolveCollisionsInelastic) {
        float coefficient = particle1->CoefficientOfRestitution;
        if (particle2->CoefficientOfRestitution < coefficient)
            coefficient = particle2->CoefficientOfRestitution;

        float impulseMagnitude =
            (-(1.0f + coefficient) * glm::dot((particle2->Velocity - particle1->Velocity),
                           collisionNormal)) /
            (particle1->InverseMass + particle2->InverseMass);

        particle1->AddImpulse(impulseMagnitude * -collisionNormal);
        particle2->AddImpulse(impulseMagnitude * collisionNormal);
    } else {
        particle1->Velocity -= 2.0f * glm::dot(particle1->Velocity, collisionNormal) * collisionNormal;
        particle2->Velocity -= 2.0f * glm::dot(particle2->Velocity, -collisionNormal) * -collisionNormal;
    }
}