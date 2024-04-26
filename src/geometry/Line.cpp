#include "Line.h"

Line::Line() : Start(-10.0f, 0.0f), End(10.0f, 0.0f), Normal(), Direction(), ProjectionOntoNormal(0.0f) {
    CalculateNormal();
}

Line::Line(glm::vec2 start, glm::vec2 end)
    : Start(start), End(end), Normal(), Direction(), ProjectionOntoNormal(0.0f) {
    CalculateNormal();
}

void Line::CalculateNormal() {
    Direction = End - Start;
    Normal = glm::normalize(glm::vec2{-Direction.y, Direction.x});
    ProjectionOntoNormal = glm::dot(Start, Normal);
}
