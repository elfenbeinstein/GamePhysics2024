#include "Line.h"
#include "core/Colors.h"
#include "core/Draw.h"

Line::Line()
    : Particle(Particle::Line, Colors::blue, 0.0f, 0.0f, 1.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, false, false, false, false, false),
      End(10.0f, 0.0f),
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
      End(end),
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
      End(end),
      Normal(),
      Direction(),
      ProjectionOntoNormal(0.0f) {
    CalculateNormal();
}

void Line::CalculateNormal() {
    Direction = End - Position;
    Normal = glm::normalize(glm::vec2{-Direction.y, Direction.x});
    ProjectionOntoNormal = glm::dot(Position, Normal);
}

void Line::Draw() {
    Draw::SetColor(Colour);
    Draw::Line(Position, End);
    Draw::SetColor(Colors::white);
}

bool Line::IsMouseOverParticle(glm::vec2 mousePosition) {
    float projectedDistance =
        glm::dot(mousePosition, Normal) - ProjectionOntoNormal;
    if (glm::abs(projectedDistance) > 0.05f)
        return false;
    float distanceToStartSquared = std::pow(Position.x - mousePosition.x, 2) +
                                   std::pow(Position.y - mousePosition.y, 2);
    float distanceToEndSquared = std::pow(End.x - mousePosition.x, 2) +
                                 std::pow(End.y - mousePosition.y, 2);

    float distanceClosestPointFromStart = glm::sqrt(
        distanceToStartSquared + glm::pow(glm::abs(projectedDistance), 2));
    float distanceClosestPointFromEnd = glm::sqrt(
        distanceToEndSquared + glm::pow(glm::abs(projectedDistance), 2));

    float lineLength = glm::distance(Position, End);

    if (distanceClosestPointFromStart > lineLength ||
        distanceClosestPointFromEnd > lineLength)
        return false;

    return true;
}

void Line::Move(glm::vec2 previousMousePosition,
                          glm::vec2 currentMousePosition) {
    Position += currentMousePosition - previousMousePosition;
    End += currentMousePosition - previousMousePosition;
    CalculateNormal();
}

float Line::GetInverseMomentOfInertia() {
    return 0.0f;
}
