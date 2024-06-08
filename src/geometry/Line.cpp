#include "Line.h"
#include "core/Colors.h"
#include "core/Draw.h"

Line::Line()
    : Particle(Particle::Line, Colors::blue, 0.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, false, false, false),
      Start(-10.0f, 0.0f),
      End(10.0f, 0.0f),
      Normal(),
      Direction(),
      ProjectionOntoNormal(0.0f) {
    CalculateNormal();
}

Line::Line(glm::vec2 start, glm::vec2 end) 
    : Particle(Particle::Line, Colors::blue, 0.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, false, false, false), 
      Start(start),
      End(end),
      Normal(),
      Direction(),
      ProjectionOntoNormal(0.0f) {
    CalculateNormal();
}

Line::Line(glm::vec2 start, glm::vec2 end, bool deleteCollidingParticle, bool canBeRemoved)
    : Particle(Particle::Line, Colors::blue, 0.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, false, deleteCollidingParticle, canBeRemoved),
      Start(start),
      End(end),
      Normal(),
      Direction(),
      ProjectionOntoNormal(0.0f) {
    CalculateNormal();
}

void Line::CalculateNormal() {
    Direction = End - Start;
    Normal = glm::normalize(glm::vec2{-Direction.y, Direction.x});
    ProjectionOntoNormal = glm::dot(Start, Normal);
}

void Line::Draw() {
    Draw::SetColor(Colour);
    Draw::Line(Start, End);
    Draw::SetColor(Colors::white);
}
