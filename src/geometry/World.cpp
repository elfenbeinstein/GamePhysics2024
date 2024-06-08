#include "World.h"

World::World() : Gravity({0, -9.81f}) {}
World::World(glm::vec2 gravity) : Gravity(gravity) {}

void World::SetCollisionCallback(CollisionCallback callback) {
    collisionCallback = callback;
}

void World::Update(std::vector<std::shared_ptr<Particle>>& particles, float deltaTime, std::vector<std::shared_ptr<Particle>>& particlesToRemove, std::vector<std::shared_ptr<Particle>>& selectedParticles) {
    for (auto& particle : particles) {
        bool skipParticle = false;
        for (auto& selected : selectedParticles)
            if (particle == selected)
                skipParticle = true;
        if (skipParticle)
            continue;
        particle->Update(deltaTime, Gravity);
    }

    for (int i = 0; i < particles.size(); ++i) {
        std::shared_ptr<Circle> circle;
        std::shared_ptr<Line> line;
        std::shared_ptr<AxisAlignedBox> box;
        if (particles[i]->Type == Particle::Circle) {
            circle = std::dynamic_pointer_cast<Circle>(particles[i]);
        } else if (particles[i]->Type == Particle::Line) {
            line = std::dynamic_pointer_cast<Line>(particles[i]);
        } else if (particles[i]->Type == Particle::AABB) {
            box = std::dynamic_pointer_cast<AxisAlignedBox>(particles[i]);
        }

        bool isColliding = false;
        glm::vec2 normal;
        float intersectionDepth;

        for (int j = i + 1; j < particles.size(); ++j) {
            std::shared_ptr<Particle> particle1;
            std::shared_ptr<Particle> particle2;

            if (particles[i]->Type == Particle::Circle &&
                particles[j]->Type == Particle::Circle) {
                isColliding =
                    IsColliding(circle,
                                std::dynamic_pointer_cast<Circle>(particles[j]),
                                intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::Circle &&
                       particles[j]->Type == Particle::Line) {
                isColliding =
                    IsColliding(circle,
                                std::dynamic_pointer_cast<Line>(particles[j]),
                                intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::Circle &&
                       particles[j]->Type == Particle::AABB) {
                isColliding =
                    IsColliding(circle,
                                std::dynamic_pointer_cast<AxisAlignedBox>(particles[j]),
                                intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::Line &&
                       particles[j]->Type == Particle::Circle) {
                isColliding =
                    IsColliding(std::dynamic_pointer_cast<Circle>(particles[j]),
                                line,
                                intersectionDepth, normal);
            } else if (particles[i]->Type == Particle::AABB &&
                       particles[j]->Type == Particle::Circle) {
                isColliding =
                    IsColliding(std::dynamic_pointer_cast<Circle>(particles[j]),
                                box,
                                intersectionDepth, normal);
            }
            if (isColliding) {
                ResolveCollision(particles[i], particles[j], intersectionDepth,
                                 normal);

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

    circle1->OnCollision();
    circle2->OnCollision();

    intersectionDepth = circle1->Radius + circle2->Radius - distance;
    if (intersectionDepth > 0) {
        glm::vec2 direction(
            (circle2->Position.x - circle1->Position.x) / distance,
            (circle2->Position.y - circle1->Position.y) / distance);
        if (circle1->Mass > 0.0f)
            circle1->Position -= direction * (intersectionDepth / 2.0f + 0.05f);
        if (circle2->Mass > 0.0f)
            circle2->Position += direction * (intersectionDepth / 2.0f + 0.05f);
    }

    glm::vec2 middle =
        glm::vec2((circle1->Position.x + circle2->Position.x) / 2.0f,
                  (circle1->Position.y + circle2->Position.y) / 2.0f);
    collisionNormal = glm::normalize(glm::vec2(
        circle1->Position.x - middle.x, circle1->Position.y - middle.y));

    return true;
}

bool World::IsColliding(std::shared_ptr<Circle> circle, std::shared_ptr<Line> line, float& intersectionDepth, glm::vec2& collisionNormal) {
    float radiusSquared = std::pow(circle->Radius, 2);
    float distanceToStartSquared =
        std::pow(line->Start.x - circle->Position.x, 2) +
        std::pow(line->Start.y - circle->Position.y, 2);
    collisionNormal = line->Normal;


    if (distanceToStartSquared <= radiusSquared) {
        intersectionDepth = std::sqrt(radiusSquared - distanceToStartSquared);
        return true;
    }

    float distanceToEndSquared = std::pow(line->End.x - circle->Position.x, 2) +
                                 std::pow(line->End.y - circle->Position.y, 2);
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

    float lineLength = glm::distance(line->Start, line->End);

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

void World::ResolveCollision(std::shared_ptr<Particle> particle1, std::shared_ptr<Particle> particle2, const float& intersectionDepth, const glm::vec2& collisionNormal) {
    if (particle1->Type == Particle::Circle &&
        particle2->Type == Particle::Circle) {
        std::shared_ptr<Circle> circle1 = std::dynamic_pointer_cast<Circle>(particle1);
        std::shared_ptr<Circle> circle2 = std::dynamic_pointer_cast<Circle>(particle2);
        float distance =
            std::sqrt(std::pow(circle2->Position.x - circle1->Position.x, 2) +
                      std::pow(circle2->Position.y - circle1->Position.y, 2));
        glm::vec2 direction(
            (circle2->Position.x - circle1->Position.x) / distance,
            (circle2->Position.y - circle1->Position.y) / distance);
        float totalMass = circle1->Mass + circle2->Mass;
        if (totalMass == 0.0f)
            return;
        float massRatio1 = circle1->Mass / totalMass;
        float massRatio2 = circle2->Mass / totalMass;
        circle1->Position -= direction * (intersectionDepth * massRatio1);
        circle2->Position += direction * (intersectionDepth * massRatio2);
        particle1->Velocity -= 2.0f * glm::dot(particle1->Velocity, collisionNormal) * collisionNormal;
        particle2->Velocity -= 2.0f * glm::dot(particle2->Velocity, -collisionNormal) * -collisionNormal;
        return;
    }
    
    
    if (particle1->Type == Particle::Circle && particle1->Mass > 0) {
        std::dynamic_pointer_cast<Circle>(particle1)->Position += intersectionDepth * collisionNormal;
        particle1->Velocity -= 2.0f * glm::dot(particle1->Velocity, collisionNormal) * collisionNormal;
    }
    if (particle2->Type == Particle::Circle && particle2->Mass > 0) {
        std::dynamic_pointer_cast<Circle>(particle2)->Position += intersectionDepth * collisionNormal;
        particle2->Velocity -= 2.0f * glm::dot(particle2->Velocity, collisionNormal) * collisionNormal;
    }

    // this did not work because I always calculate the normal and depth and stuff with using the circle as particle 1 so I need to do this accordingly
    /*particle1->Velocity -= 2.0f * glm::dot(particle1->Velocity, collisionNormal) * collisionNormal;
    particle2->Velocity -= 2.0f * glm::dot(particle2->Velocity, -collisionNormal) * -collisionNormal;*/
}