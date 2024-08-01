#include "Line.h"
#include "core/Colors.h"
#include "core/Draw.h"

Line::Line()
    : Particle(Particle::Line, Colors::blue, 0.0f, 0.0f, 1.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, false, false, false, false, false),
      EndOffset(10.0f, 0.0f),
      Normal(),
      Direction(),
      ProjectionOntoNormal(0.0f) {
    CalculateNormal();
}

Line::Line(glm::vec2 start,
           glm::vec2 end,
           bool deleteCollidingParticle,
           bool canBeRemoved)
    : Particle(Particle::Line,
               Colors::blue,
               0.0f,
               0.0f,
               1.0f,
               glm::vec2{0.0f, 0.0f},
               glm::vec2{0.0f, 0.0f},
               start,
               false,
               deleteCollidingParticle,
               canBeRemoved,
               false,
               false),
      EndOffset(end),
      Normal(),
      Direction(),
      ProjectionOntoNormal(0.0f) {
    CalculateNormal();
}

Line::Line(glm::vec2 start,
           glm::vec2 end,
           float mass,
           float damping,
           float coefficientOfRestitution,
           glm::vec2 vel,
           glm::vec2 totalForce,
           glm::vec2 position,
           bool removeAfterCol,
           bool deleteCollidingParticle,
           bool canBeRemoved,
           bool canBeMoved,
           bool canAddImpulse,
           Color color)
    : Particle(Particle::Line,
               color,
               mass,
               damping,
               coefficientOfRestitution,
               vel,
               totalForce,
               start,
               removeAfterCol,
               deleteCollidingParticle,
               canBeRemoved,
               canBeMoved,
               canAddImpulse),
      EndOffset(end),
      Normal(),
      Direction(),
      ProjectionOntoNormal(0.0f) {
    CalculateNormal();
}

void Line::CalculateNormal() {
    Direction = EndOffset - Position;
    Normal = glm::normalize(glm::vec2{-Direction.y, Direction.x});
    ProjectionOntoNormal = glm::dot(Position, Normal);
}

void Line::Draw() {
    Draw::SetColor(Colour);
    Draw::Line(Position, EndOffset);
    Draw::SetColor(Colors::white);
}
