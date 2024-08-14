#include "World.h"

World::World()
    : Gravity({0, -9.81f}),
      ApplyDrag(false),
      ResolveCollisionsInelastic(false),
      ResolveCollisionsWithImpulsiveTorque(false) {}

World::World(glm::vec2 gravity)
    : Gravity(gravity),
      ApplyDrag(false),
      ResolveCollisionsInelastic(false),
      ResolveCollisionsWithImpulsiveTorque(false) {}

World::World(glm::vec2 gravity, bool applyDrag, bool resolveCollisionsInelastic, bool resolveCollisionsWithImpulsiveTorque)
    : Gravity(gravity),
      ApplyDrag(applyDrag),
      ResolveCollisionsInelastic(resolveCollisionsInelastic),
      ResolveCollisionsWithImpulsiveTorque(resolveCollisionsWithImpulsiveTorque) {}

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
        CollisionManifold collisionManifold = CollisionManifold({0.0f, 0.0f}, 0.0f);

        for (int j = i + 1; j < particles.size(); ++j) {
            if (particles[i]->Type == Particle::Circle &&
                particles[j]->Type == Particle::Circle) {
                isColliding =
                    IsColliding(std::dynamic_pointer_cast<Circle>(particles[i]),
                                std::dynamic_pointer_cast<Circle>(particles[j]),
                                collisionManifold);
            } else if (particles[i]->Type == Particle::Circle &&
                       particles[j]->Type == Particle::Line) {
                isColliding =
                    IsColliding(std::dynamic_pointer_cast<Circle>(particles[i]),
                                std::dynamic_pointer_cast<Line>(particles[j]),
                                collisionManifold);
                collisionManifold.CollisionNormal = -collisionManifold.CollisionNormal;
            } else if (particles[i]->Type == Particle::Circle &&
                       particles[j]->Type == Particle::AABB) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Circle>(particles[i]),
                    std::dynamic_pointer_cast<AxisAlignedBox>(particles[j]),
                    collisionManifold);
            } else if (particles[i]->Type == Particle::Circle &&
                       particles[j]->Type == Particle::Rectangle) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[j]),
                    std::dynamic_pointer_cast<Circle>(particles[i]),
                    collisionManifold);
            } else if (particles[i]->Type == Particle::Line &&
                       particles[j]->Type == Particle::Circle) {
                isColliding =
                    IsColliding(std::dynamic_pointer_cast<Circle>(particles[j]),
                                std::dynamic_pointer_cast<Line>(particles[i]),
                                collisionManifold);
            } else if (particles[i]->Type == Particle::Line &&
                       particles[j]->Type == Particle::Rectangle) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[j]),
                    std::dynamic_pointer_cast<Line>(particles[i]),
                    collisionManifold);
            } else if (particles[i]->Type == Particle::AABB &&
                       particles[j]->Type == Particle::Circle) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Circle>(particles[j]),
                    std::dynamic_pointer_cast<AxisAlignedBox>(particles[i]),
                    collisionManifold);
                collisionManifold.CollisionNormal = -collisionManifold.CollisionNormal;
            } else if (particles[i]->Type == Particle::AABB &&
                       particles[j]->Type == Particle::Rectangle) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[j]),
                    std::dynamic_pointer_cast<AxisAlignedBox>(particles[i]),
                    collisionManifold);
                collisionManifold.CollisionNormal = -collisionManifold.CollisionNormal;
            } else if (particles[i]->Type == Particle::Rectangle &&
                       particles[j]->Type == Particle::Rectangle) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[i]),
                    std::dynamic_pointer_cast<Rectangle>(particles[j]),
                    collisionManifold);
            } else if (particles[i]->Type == Particle::Rectangle &&
                       particles[j]->Type == Particle::Circle) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[i]),
                    std::dynamic_pointer_cast<Circle>(particles[j]),
                    collisionManifold);
                collisionManifold.CollisionNormal = -collisionManifold.CollisionNormal;
            } else if (particles[i]->Type == Particle::Rectangle &&
                       particles[j]->Type == Particle::Line) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[i]),
                    std::dynamic_pointer_cast<Line>(particles[j]),
                    collisionManifold);
                collisionManifold.CollisionNormal = -collisionManifold.CollisionNormal;
            } else if (particles[i]->Type == Particle::Rectangle &&
                       particles[j]->Type == Particle::AABB) {
                isColliding = IsColliding(
                    std::dynamic_pointer_cast<Rectangle>(particles[i]),
                    std::dynamic_pointer_cast<AxisAlignedBox>(particles[j]),
                    collisionManifold);
            }

            if (isColliding) {
                ResolveCollision(particles[i], particles[j], collisionManifold);

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

bool World::IsColliding(std::shared_ptr<Circle> circle1, std::shared_ptr<Circle> circle2, CollisionManifold& collisionManifold) {
    float distance =
        std::sqrt(std::pow(circle2->Position.x - circle1->Position.x, 2) +
                  std::pow(circle2->Position.y - circle1->Position.y, 2));
    if (distance > circle1->Radius + circle2->Radius)
        return false;
    collisionManifold.IntersectionDepth = circle1->Radius + circle2->Radius - distance;
    collisionManifold.CollisionNormal = glm::normalize(glm::vec2(circle2->Position.x - circle1->Position.x,
                                               circle2->Position.y - circle1->Position.y));
    collisionManifold.ContactPointCount = 1;
    glm::vec2 direction = glm::normalize(circle2->Position - circle1->Position);
    collisionManifold.ContactPoint1 =
        circle1->Position + circle1->Radius * direction;

    return true;
}

bool World::IsColliding(std::shared_ptr<Circle> circle, std::shared_ptr<Line> line, CollisionManifold& collisionManifold) {
    float radiusSquared = std::pow(circle->Radius, 2);
    float distanceToStartSquared =
        std::pow(line->Position.x - circle->Position.x, 2) +
        std::pow(line->Position.y - circle->Position.y, 2);
    collisionManifold.CollisionNormal = line->Normal;


    if (distanceToStartSquared <= radiusSquared) {
        collisionManifold.IntersectionDepth = std::sqrt(radiusSquared - distanceToStartSquared);
        collisionManifold.ContactPoint1 = line->Position;
        collisionManifold.ContactPointCount = 1;
        return true;
    }

    float distanceToEndSquared = std::pow(line->End.x - circle->Position.x, 2) +
                                 std::pow(line->End.y - circle->Position.y, 2);
    if (distanceToEndSquared <= radiusSquared) {
        collisionManifold.IntersectionDepth = std::sqrt(radiusSquared - distanceToEndSquared);
        collisionManifold.ContactPoint1 = line->End;
        collisionManifold.ContactPointCount = 1;
        return true;
    }

    float projectedDistance =
        glm::dot(circle->Position, collisionManifold.CollisionNormal) - line->ProjectionOntoNormal;
    if (glm::abs(projectedDistance) > circle->Radius)
        return false;

    float distanceClosestPointFromStart = glm::sqrt(
        distanceToStartSquared + glm::pow(glm::abs(projectedDistance), 2));
    float distanceClosestPointFromEnd = glm::sqrt(
        distanceToEndSquared + glm::pow(glm::abs(projectedDistance), 2));

    float lineLength = glm::distance(line->Position, line->End);

    if (distanceClosestPointFromStart > lineLength ||
        distanceClosestPointFromEnd > lineLength)
        return false;

    collisionManifold.IntersectionDepth = circle->Radius - projectedDistance;
    if (projectedDistance < 0) {
        collisionManifold.IntersectionDepth = circle->Radius + projectedDistance;
        collisionManifold.CollisionNormal = -collisionManifold.CollisionNormal;
    }

    float projectionLength = glm::dot((circle->Position - line->Position),
                                      glm::normalize(line->Normal));
    collisionManifold.ContactPoint1 =
        line->Position + projectionLength * glm::normalize(line->Direction);
    collisionManifold.ContactPointCount = 1;

    return true;
}

bool World::IsColliding(std::shared_ptr<Circle> circle, std::shared_ptr<AxisAlignedBox>box, CollisionManifold& collisionManifold) {
    glm::vec2 closestPoint = box->ClosestPoint(circle->Position);
    float distanceSquared = std::pow(circle->Position.x - closestPoint.x, 2) +
                            std::pow(circle->Position.y - closestPoint.y, 2);

    if (distanceSquared > std::pow(circle->Radius, 2))
        return false;

    collisionManifold.IntersectionDepth = circle->Radius - std::sqrt(distanceSquared);
    collisionManifold.CollisionNormal = glm::normalize(closestPoint - circle->Position);

    glm::vec2 halfExtends = {0.0f, 0.0f};
    halfExtends.x = (box->TopRight.x - box->Position.x) / 2.0f;
    halfExtends.y = (box->TopRight.y - box->Position.y) / 2.0f;
    glm::vec2 pos = box->Position + halfExtends;
    collisionManifold.ContactPoint1 = PhysicsUtils::FindContactPointCircleRect(circle->Position, circle->Radius, pos, 0.0f, halfExtends);
    collisionManifold.ContactPointCount = 1;
    
    return true;
}

bool World::IsColliding(std::shared_ptr<Rectangle> rectA, std::shared_ptr<Rectangle> rectB, CollisionManifold& collisionManifold) {
    bool isColliding = PhysicsUtils::IsCollidingRectRect(
        rectA->Position, rectA->Angle, rectA->HalfExtends, rectB->Position,
        rectB->Angle, rectB->HalfExtends, collisionManifold.CollisionNormal, collisionManifold.IntersectionDepth);

    if (isColliding) {
        PhysicsUtils::FindContactPointsRectRect(
            rectA->Position, rectA->Angle, rectA->HalfExtends, rectB->Position,
            rectB->Angle, rectB->HalfExtends, collisionManifold.ContactPoint1,
            collisionManifold.ContactPoint2,
            collisionManifold.ContactPointCount);
    }
    return isColliding;
}

bool World::IsColliding(std::shared_ptr<Rectangle> rectangle, std::shared_ptr<Line> line, CollisionManifold& collisionManifold) {
    float minA, maxA, minB, maxB;
    std::vector<glm::vec2> rectPoints = Math::GetRectangleWorldPoints(
        rectangle->Position, rectangle->Angle, rectangle->HalfExtends);
    std::vector<glm::vec2> linePoints = {line->Position, line->End};

    collisionManifold.IntersectionDepth = FLT_MAX;

    for (int i = 0; i < rectPoints.size(); i++) {
        glm::vec2 a = rectPoints[i];
        glm::vec2 b = rectPoints[(i + 1) % rectPoints.size()];

        glm::vec2 edge = b - a;
        glm::vec2 normal = glm::normalize(glm::vec2(edge.y, -edge.x));

        PhysicsUtils::ProjectPoints(rectPoints, normal, minA, maxA);
        PhysicsUtils::ProjectPoints(linePoints, normal, minB, maxB);

        if (minA >= maxB || minB >= maxA) {
            return false;
        }

        float depth = std::min(maxA - minB, maxB - minA);
        if (depth < collisionManifold.IntersectionDepth) {
            collisionManifold.IntersectionDepth = depth;
            collisionManifold.CollisionNormal = normal;
        }
    }

    PhysicsUtils::ProjectPoints(rectPoints, line->Normal, minA, maxA);
    PhysicsUtils::ProjectPoints(linePoints, line->Normal, minB, maxB);

    if (minA >= maxB || minB >= maxA) {
        return false;
    }

    float depth = std::min(maxA - minB, maxB - minA);
    if (depth < collisionManifold.IntersectionDepth) {
        collisionManifold.IntersectionDepth = depth;
        collisionManifold.CollisionNormal = line->Normal;
    }

    glm::vec2 rectCenter =
        (rectPoints[0] + rectPoints[1] + rectPoints[2] + rectPoints[3]) / 4.0f;
    if (glm::dot(rectCenter - line->Position, collisionManifold.CollisionNormal) < 0) {
        collisionManifold.CollisionNormal = -collisionManifold.CollisionNormal;
    }

    PhysicsUtils::FindContactPointsPolygonPolygon(
        rectPoints, linePoints, collisionManifold.ContactPoint1,
        collisionManifold.ContactPoint2, collisionManifold.ContactPointCount);

    return true;
}

bool World::IsColliding(std::shared_ptr<Rectangle> rectangle, std::shared_ptr<AxisAlignedBox> box, CollisionManifold& collisionManifold) {
    glm::vec2 halfExtends = {0.0f, 0.0f};
    halfExtends.x = (box->TopRight.x - box->Position.x) / 2.0f;
    halfExtends.y = (box->TopRight.y - box->Position.y) / 2.0f;
    glm::vec2 pos = box->Position + halfExtends;

    bool isColliding = PhysicsUtils::IsCollidingRectRect(
        rectangle->Position, rectangle->Angle, rectangle->HalfExtends, pos,
        0.0f, halfExtends, collisionManifold.CollisionNormal, collisionManifold.IntersectionDepth);

    if (isColliding) {
        PhysicsUtils::FindContactPointsRectRect(
            rectangle->Position, rectangle->Angle, rectangle->HalfExtends, pos,
            0.0f, halfExtends, collisionManifold.ContactPoint1,
            collisionManifold.ContactPoint2,
            collisionManifold.ContactPointCount);
    }

    return isColliding;
}

bool World::IsColliding(std::shared_ptr<Rectangle> rectangle, std::shared_ptr<Circle> circle, CollisionManifold& collisionManifold) {
    bool isColliding = PhysicsUtils::IsCollidingCircleRect(
        circle->Position, circle->Radius, rectangle->Position, rectangle->Angle,
        rectangle->HalfExtends, collisionManifold.CollisionNormal, collisionManifold.IntersectionDepth);

    if (isColliding) {
        collisionManifold.ContactPoint1 =
            PhysicsUtils::FindContactPointCircleRect(
                circle->Position, circle->Radius, rectangle->Position,
                rectangle->Angle, rectangle->HalfExtends);
        collisionManifold.ContactPointCount = 1;
    }

    return isColliding;
}

void World::ResolveCollision(std::shared_ptr<Particle> particle1, std::shared_ptr<Particle> particle2, CollisionManifold& collisionManifold) {
    float totalMass = particle1->Mass + particle2->Mass;
    if (totalMass < 0.0001f)
        return;
    particle1->OnCollision();
    particle2->OnCollision();

    if (particle2->Mass < 0.0001f) {
        particle1->Position -= collisionManifold.CollisionNormal * collisionManifold.IntersectionDepth;
    } else if (particle1->Mass < 0.0001f) {
        particle2->Position += collisionManifold.CollisionNormal * collisionManifold.IntersectionDepth;
    } else {
        float massRatio1 = particle1->Mass / totalMass;
        float massRatio2 = particle2->Mass / totalMass;

        particle1->Position -= collisionManifold.CollisionNormal * (collisionManifold.IntersectionDepth * massRatio2);
        particle2->Position += collisionManifold.CollisionNormal * (collisionManifold.IntersectionDepth * massRatio1);
    }

    if (ResolveCollisionsWithImpulsiveTorque && collisionManifold.ContactPointCount > 0) {
        std::shared_ptr<RigidBody> rigidBody1 = nullptr;
        std::shared_ptr<RigidBody> rigidBody2 = nullptr;

        if (particle1->Type == Particle::Circle ||
            particle1->Type == Particle::Rectangle) {
            rigidBody1 = std::dynamic_pointer_cast<RigidBody>(particle1);
        }

        if (particle2->Type == Particle::Circle ||
            particle2->Type == Particle::Rectangle) {
            rigidBody2 = std::dynamic_pointer_cast<RigidBody>(particle2);
        }

        if (rigidBody1 != nullptr && rigidBody2 != nullptr) {
            ResolveCollisionWithImpulsiveTorque(rigidBody1, rigidBody2, collisionManifold);
        } else if (rigidBody1 != nullptr) {
            ResolveCollisionWithImpulsiveTorque(rigidBody1, particle2, collisionManifold);
        } else if (rigidBody2 != nullptr) {
            //collisionManifold.CollisionNormal = -collisionManifold.CollisionNormal;
            ResolveCollisionWithImpulsiveTorque(rigidBody2, particle1, collisionManifold);
        }
    } else if (ResolveCollisionsInelastic) {
        float coefficient = particle1->CoefficientOfRestitution;
        if (particle2->CoefficientOfRestitution < coefficient)
            coefficient = particle2->CoefficientOfRestitution;

        float impulseMagnitude =
            (-(1.0f + coefficient) * glm::dot((particle2->Velocity - particle1->Velocity),
                      collisionManifold.CollisionNormal)) /
            (particle1->InverseMass + particle2->InverseMass);

        particle1->AddImpulse(impulseMagnitude * -collisionManifold.CollisionNormal);
        particle2->AddImpulse(impulseMagnitude * collisionManifold.CollisionNormal);
    } else {
        particle1->Velocity -= 2.0f * glm::dot(particle1->Velocity, collisionManifold.CollisionNormal) * collisionManifold.CollisionNormal;
        particle2->Velocity -= 2.0f * glm::dot(particle2->Velocity, -collisionManifold.CollisionNormal) * -collisionManifold.CollisionNormal;
    }
}

void World::ResolveCollisionWithImpulsiveTorque(std::shared_ptr<RigidBody> rigidBodyA, std::shared_ptr<RigidBody> rigidBodyB, CollisionManifold& collisionManifold) {
    float coefficient = rigidBodyA->CoefficientOfRestitution;
    if (rigidBodyB->CoefficientOfRestitution < coefficient)
        coefficient = rigidBodyB->CoefficientOfRestitution;
    
    // contact point 1
    glm::vec2 rA = collisionManifold.ContactPoint1 - rigidBodyA->Position;
    glm::vec2 totalVelocityA = rigidBodyA->Velocity + rigidBodyA->AngularVelocity * glm::vec2{-rA.y, rA.x};
    glm::vec2 rB = collisionManifold.ContactPoint1 - rigidBodyB->Position;
    glm::vec2 totalVelocityB = rigidBodyB->Velocity + rigidBodyB->AngularVelocity * glm::vec2{-rB.y, rB.x};

    float numerator = (-(1.0f + coefficient) * 
        glm::dot((totalVelocityB - totalVelocityA), collisionManifold.CollisionNormal));
    float denominator =
        (rigidBodyA->InverseMass + rigidBodyB->InverseMass) + 
        (glm::pow(glm::dot(rA, collisionManifold.CollisionNormal), 2) * rigidBodyA->InverseMomentOfInertia) + 
        (glm::pow(glm::dot(rB, collisionManifold.CollisionNormal), 2) * rigidBodyB->InverseMomentOfInertia);
    float impulseMagnitude1 = (numerator / denominator) / collisionManifold.ContactPointCount;

    // contact point 2
    if (collisionManifold.ContactPointCount > 1) {
        rA = collisionManifold.ContactPoint2 - rigidBodyA->Position;
        totalVelocityA = rigidBodyA->Velocity + rigidBodyA->AngularVelocity * glm::vec2{-rA.y, rA.x};
        rB = collisionManifold.ContactPoint2 - rigidBodyB->Position;
        totalVelocityB = rigidBodyB->Velocity + rigidBodyB->AngularVelocity * glm::vec2{-rB.y, rB.x};
        numerator = (-(1.0f + coefficient) * glm::dot((totalVelocityB - totalVelocityA), collisionManifold.CollisionNormal));
        denominator =
            (rigidBodyA->InverseMass + rigidBodyB->InverseMass) +
            (glm::pow(glm::dot(rA, collisionManifold.CollisionNormal), 2) * rigidBodyA->InverseMomentOfInertia) +
            (glm::pow(glm::dot(rB, collisionManifold.CollisionNormal), 2) * rigidBodyB->InverseMomentOfInertia);
        float impulseMagnitude2 = (numerator / denominator) / collisionManifold.ContactPointCount;
         
        rigidBodyA->AddImpulseWithTorque(impulseMagnitude2 * -collisionManifold.CollisionNormal, collisionManifold.ContactPoint2);
        rigidBodyB->AddImpulseWithTorque(impulseMagnitude2 * collisionManifold.CollisionNormal, collisionManifold.ContactPoint2);
    }

    rigidBodyA->AddImpulseWithTorque(impulseMagnitude1 * -collisionManifold.CollisionNormal, collisionManifold.ContactPoint1);
    rigidBodyB->AddImpulseWithTorque(impulseMagnitude1 * collisionManifold.CollisionNormal, collisionManifold.ContactPoint1);
}

void World::ResolveCollisionWithImpulsiveTorque(std::shared_ptr<RigidBody> rigidBody, std::shared_ptr<Particle> particle, CollisionManifold& collisionManifold) {
    return;
    float coefficient = rigidBody->CoefficientOfRestitution;
    if (particle->CoefficientOfRestitution < coefficient)
        coefficient = particle->CoefficientOfRestitution;

    // contact point 1
    glm::vec2 rA = collisionManifold.ContactPoint1 - rigidBody->Position;
    glm::vec2 totalVelocityA = rigidBody->Velocity + rigidBody->AngularVelocity * glm::vec2{-rA.y, rA.x};
    glm::vec2 rB = collisionManifold.ContactPoint1 - particle->Position;
    glm::vec2 totalVelocityB = particle->Velocity;

    float numerator = (-(1.0f + coefficient) * 
        glm::dot((totalVelocityB - totalVelocityA), collisionManifold.CollisionNormal));
    float denominator =
        (rigidBody->InverseMass + particle->InverseMass) + 
        (glm::pow(glm::dot(rA, collisionManifold.CollisionNormal), 2) * rigidBody->InverseMomentOfInertia) + 
        (glm::pow(glm::dot(rB, collisionManifold.CollisionNormal), 2));
    float impulseMagnitude1 = (numerator / denominator) / collisionManifold.ContactPointCount;

    // contact point 2
    if (collisionManifold.ContactPointCount > 1) {
        rA = collisionManifold.ContactPoint2 - rigidBody->Position;
        totalVelocityA = rigidBody->Velocity + rigidBody->AngularVelocity * glm::vec2{-rA.y, rA.x};
        rB = collisionManifold.ContactPoint2 - particle->Position;
        totalVelocityB = particle->Velocity;
        numerator = (-(1.0f + coefficient) * glm::dot((totalVelocityB - totalVelocityA), collisionManifold.CollisionNormal));
        denominator =
            (rigidBody->InverseMass + particle->InverseMass) +
            (glm::pow(glm::dot(rA, collisionManifold.CollisionNormal), 2) * rigidBody->InverseMomentOfInertia) +
            (glm::pow(glm::dot(rB, collisionManifold.CollisionNormal), 2));
        float impulseMagnitude2 = (numerator / denominator) / collisionManifold.ContactPointCount;

        rigidBody->AddImpulseWithTorque(impulseMagnitude2 * -collisionManifold.CollisionNormal, collisionManifold.ContactPoint2);
        particle->AddImpulse(impulseMagnitude2 * collisionManifold.CollisionNormal);
    }

    float impulseMagnitude =
            (-(1.0f + coefficient) * glm::dot((particle->Velocity - rigidBody->Velocity),
                      collisionManifold.CollisionNormal)) /
            (rigidBody->InverseMass + particle->InverseMass);

    /*rigidBody->AddImpulseWithTorque(impulseMagnitude1 * -collisionManifold.CollisionNormal, collisionManifold.ContactPoint1);
    particle->AddImpulse(impulseMagnitude1 * collisionManifold.CollisionNormal);*/

    rigidBody->AddImpulse(impulseMagnitude * -collisionManifold.CollisionNormal);
    particle->AddImpulse(impulseMagnitude * collisionManifold.CollisionNormal);

}