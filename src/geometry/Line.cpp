#include "Line.h"

Line::Line() : Start(-10.0f, 0.0f), End(10.0f, 0.0f) {}

Line::Line(glm::vec2 start, glm::vec2 end) : Start(start), End(end) {}
