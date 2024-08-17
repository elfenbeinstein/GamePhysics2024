#include "Rectangle.h"
#include "core/Colors.h"
#include "core/Simple2DScene.h"
#include "core/Draw.h"
#include <vector>

Rectangle::Rectangle(glm::vec2 halfExtends,
                     glm::vec2 pos,
                     glm::vec2 vel,
                     float mass,
                     float damping,
                     float coefficientOfRestitution,
                     float angle,
                     float angularVelocity,
                     float angularDamp,
                     bool removeAfterCollision,
                     bool removeOtherAfterCollision,
                     bool canBeRemoved,
                     bool canBeMovedByMouse,
                     bool canAddImpulseByMouse,
                     Color colour)
    : RigidBody(Particle::Rectangle,
                colour,
                mass,
                damping,
                coefficientOfRestitution,
                angle,
                angularDamp,
                angularVelocity,
                PhysicsUtils::GetMomentOfInertiaRect(halfExtends, mass),
                vel,
                {0.0f, 0.0f},
                pos,
                removeAfterCollision,
                removeOtherAfterCollision,
                canBeRemoved,
                canBeMovedByMouse,
                canAddImpulseByMouse),
      HalfExtends(halfExtends) {}

void Rectangle::Draw() {
    Draw::SetColor(Colour);
    if (MagneticFieldStrength > 0.0f)
        Draw::Rect(Position, Angle, HalfExtends, true);
    else
        Draw::Rect(Position, Angle, HalfExtends);
    Draw::SetColor(Colors::white);
}

bool Rectangle::IsMouseOverParticle(glm::vec2 mousePosition) {
    std::vector<glm::vec2> corners = Math::GetRectangleWorldPoints(Position, Angle, HalfExtends);
    std::vector<glm::vec2> axes = {glm::normalize(corners[1] - corners[0]),
                                   glm::normalize(corners[3] - corners[0])};

    for (const auto& axis : axes) {
        float min = glm::dot(corners[0], axis);
        float max = min;
        for (size_t i = 1; i < corners.size(); ++i) {
            float projection = glm::dot(corners[i], axis);
            if (projection < min)
                min = projection;
            if (projection > max)
                max = projection;
        }
        float mouseProjection = glm::dot(mousePosition, axis);

        if (max < mouseProjection || mouseProjection < min)
            return false;
    }

    return true;
}