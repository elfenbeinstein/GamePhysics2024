#include "FinalAssignment.h"

#include "imgui.h"
#include "core/Draw.h"
#include <string>

FinalAssignment::FinalAssignment()
    : playerInput(PlayerInput::InputType::Move, true),
      world(glm::vec2{0, -9.81f}, true, true, true),
      particles(),
      useGravity(true),
      damping(1.0f),
      coefficientOfRestitution(0.5f),
      angularDamp(0.5f),
      magnet(nullptr),
      playerLives(3),
      pointsA(),
      pointsB(),
      polygonAPoints(),
      polygonBPoints(),
      polygonAPos(),
      polygonBPos(),
      gameWon(false), 
      time(0.0f),
      amountToWin(1) {
    SetUp();
}

void FinalAssignment::OnEnable() {}

void FinalAssignment::OnDisable() {}

void FinalAssignment::Update(float deltaTime) {
    if (!gameWon)
        time += deltaTime;

    std::vector<std::shared_ptr<Particle>> particlesToRemove;
    playerInput.Update(particles);

    if (magnet && playerInput.selectedParticle != nullptr) {
        for (auto& particle : particles) {
            if (particle->Type != Particle::Rectangle)
                continue;
            std::shared_ptr<RigidBody> rb =
                std::dynamic_pointer_cast<Rectangle>(particle);
            if (rb == nullptr || rb == magnet)
                continue;

            glm::vec2 magneticForce = CalculateMagneticForce(magnet, rb);
            glm::vec2 acceleration = magneticForce * particle->InverseMass;

            // Apply the magnetic force to the velocity
            particle->Velocity += acceleration * deltaTime;
        }
    }

    world.Update(particles, deltaTime, particlesToRemove,
                 playerInput.selectedParticle);

    int correctParticles = 0;

    for (auto& particle : particles) {
        if (particle->Type != Particle::Rectangle)
            continue;
        if (particle->Mass == 0.0f)
            continue;
        if (particle->Position.y < -15.0f)
            particlesToRemove.push_back(particle);
        float depth;
        glm::vec2 normal;
        std::shared_ptr<Rectangle> rect =
            std::dynamic_pointer_cast<Rectangle>(particle);
        std::vector<glm::vec2> rectPoints = Math::GetRectangleWorldPoints(
            rect->Position, rect->Angle, rect->HalfExtends);
        if (PhysicsUtils::IsCollidingPolygonPolygon(polygonAPos, polygonAPoints,
                                                    rect->Position, rectPoints,
                                                    normal, depth)) {
            if (rect->Colour == Colors::green)
                correctParticles++;
        }
        if (PhysicsUtils::IsCollidingPolygonPolygon(polygonBPos, polygonBPoints,
                                                    rect->Position, rectPoints,
                                                    normal, depth)) {
            if (rect->Colour == Colors::yellow)
                correctParticles++;
        }
    }

    gameWon = correctParticles == amountToWin;

    playerLives -= particlesToRemove.size();

    if (playerLives == 0) {
        SetUp();
    }
    amountToWin -= particlesToRemove.size();

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [&](const std::shared_ptr<Particle>& particle) {
                           return std::find(particlesToRemove.begin(),
                                            particlesToRemove.end(),
                                            particle) !=
                                  particlesToRemove.end();
                       }),
        particles.end());

    for (auto& particle : particles) {
        particle->CoefficientOfRestitution = coefficientOfRestitution;
        particle->Damping = damping;
        if (particle->Type == Particle::Rectangle ||
            particle->Type == Particle::Circle) {
            std::shared_ptr<RigidBody> rb =
                std::dynamic_pointer_cast<RigidBody>(particle);
            if (rb == nullptr)
                continue;
            rb->AngularDamp = angularDamp;
        }
    }
}

void FinalAssignment::Draw() {
    for (auto& particle : particles) {
        particle->Draw();
    }

    Draw::SetColor(Colors::green);
    Draw::Polygon({0.0f, 0.0f}, pointsA, 4);
    Draw::SetColor(Colors::yellow);
    Draw::Polygon({0.0f, 0.0f}, pointsB, 4);
    Draw::SetColor(Colors::white);

    magnet->Draw();
    playerInput.Draw();
}

void FinalAssignment::DrawGUI() {
    ImGui::Begin("Inspector");
    ImGui::DragFloat("Magnetic Field Strength", &magnet->MagneticFieldStrength, 0.1f);
    ImGui::DragFloat("Damping", &damping, 0.01f);
    ImGui::DragFloat("CoR", &coefficientOfRestitution, 0.01f);
    ImGui::DragFloat("Angular Damp", &angularDamp, 0.01f);
    ImGui::Checkbox("Use Gravity", &useGravity);
    if (useGravity)
        world.Gravity = glm::vec2{0, -9.81f};
    else
        world.Gravity = glm::vec2{0.0f, 0.0f};

    ImGui::Spacing();
    if (ImGui::Button("Reset")) {
        SetUp();
    }

    ImGui::Spacing();
    if (!gameWon) {
        ImGui::Text("Click on magnet to use it");
        ImGui::Text("The white square is the magnet");
        ImGui::Text("Sort the squares on the left by color");
        ImGui::Text("Red lines kill squares");
        ImGui::Text(
            ("You have " + std::to_string(playerLives) + " lives remaining")
                .c_str());
        ImGui::Text(
            ("Time spent: " + std::to_string(time) + "s")
                .c_str());
    } else {
        ImGui::Text("Good job!");
        ImGui::Text(
            ("You finished with " + std::to_string(playerLives) + " lives remaining")
                .c_str());
        ImGui::Text(("Time spent: " + std::to_string(time) + "s").c_str());
    }
    
    ImGui::End();
}

void FinalAssignment::SetUp() {
    particles.clear();
    playerLives = 3;
    time = 0.0f;
    gameWon = false;
    amountToWin = 6;

    //floor
    glm::vec2 startLeft(-10.0f, 0.0f);
    glm::vec2 startRight(-2.5f, 0.0f);
    glm::vec2 downLeft(0.0f, -10.0f);
    glm::vec2 downRight(7.5f, -10.0f);
    glm::vec2 middleLeft(10.0f, -4.0f);
    glm::vec2 middleRight(17.0f, -4.0f);
    glm::vec2 endLeft(19.0f, 1.0f);
    glm::vec2 endRight(27.0f, 1.0f);
    glm::vec2 startCeilingLeft(-10.0f, 6.0f);
    glm::vec2 startCeilingRight(5.5f, 6.0f);
    glm::vec2 ceilingMiddleLeft(8.5f, 2.0f);
    glm::vec2 ceilingMiddleRight(15.5f, 2.0f);
    glm::vec2 ceilingEndLeft(17.5f, 5.0f);
    glm::vec2 ceilingEndRight(27.0f, 5.0f);

    Line start = Line(startLeft, startRight);
    Line startDown = Line(startRight, downLeft, true, false);
    startDown.Colour = Colors::red;
    Line down = Line(downLeft, downRight, true, false);
    down.Colour = Colors::red;
    Line downUp = Line(downRight, middleLeft, true, false);
    downUp.Colour = Colors::red;
    Line middle = Line(middleLeft, middleRight);
    Line middleUp = Line(middleRight, endLeft, true, false);
    middleUp.Colour = Colors::red;
    Line end = Line(endLeft, endRight);
    Line startUpCeiling = Line(startLeft, startCeilingLeft);
    Line ceilingStart = Line(startCeilingLeft, startCeilingRight);
    Line ceilingStartToMid = Line(startCeilingRight, ceilingMiddleLeft);
    Line ceilingMid = Line(ceilingMiddleLeft, ceilingMiddleRight, true, false);
    ceilingMid.Colour = Colors::red;
    Line ceilingMidToEnd = Line(ceilingMiddleRight, ceilingEndLeft, true, false);
    ceilingMidToEnd.Colour = Colors::red;
    Line ceilingEnd = Line(ceilingEndLeft, ceilingEndRight, true, false);
    ceilingEnd.Colour = Colors::red;
    Line ceilingEndDown = Line(ceilingEndRight, endRight, true, false);
    ceilingEndDown.Colour = Colors::red;

    particles.push_back(std::make_shared<Line>(start));
    particles.push_back(std::make_shared<Line>(startDown));
    particles.push_back(std::make_shared<Line>(down));
    particles.push_back(std::make_shared<Line>(downUp));
    particles.push_back(std::make_shared<Line>(middle));
    particles.push_back(std::make_shared<Line>(middleUp));
    particles.push_back(std::make_shared<Line>(end));
    particles.push_back(std::make_shared<Line>(startUpCeiling));
    particles.push_back(std::make_shared<Line>(ceilingStart));
    particles.push_back(std::make_shared<Line>(ceilingStartToMid));
    particles.push_back(std::make_shared<Line>(ceilingMid));
    particles.push_back(std::make_shared<Line>(ceilingMidToEnd));
    particles.push_back(std::make_shared<Line>(ceilingEnd));
    particles.push_back(std::make_shared<Line>(ceilingEndDown));

    // containers
    glm::vec2 startContainerA(-9.5f, 1.2f);
    glm::vec2 startContainerB(-9.0f, 0.0f);
    glm::vec2 startContainerC(-4.5f, 0.0f);
    glm::vec2 startContainerD(-4.0f, 1.2f);
    Line startContainerLeft = Line(startContainerA, startContainerB);
    Line startContainerRight = Line(startContainerC, startContainerD);
    particles.push_back(std::make_shared<Line>(startContainerLeft));
    particles.push_back(std::make_shared<Line>(startContainerRight));

    glm::vec2 midContainerA(11.0f, -2.8f);
    glm::vec2 midContainerB(11.5f, -4.0f);
    glm::vec2 midContainerC(15.0f, -4.0f);
    glm::vec2 midContainerD(15.5f, -2.8f);
    Line midContainerLeft = Line(midContainerA, midContainerB);
    Line midContainerRight = Line(midContainerC, midContainerD);
    midContainerLeft.Colour = Colors::green;
    midContainerRight.Colour = Colors::green;
    particles.push_back(std::make_shared<Line>(midContainerLeft));
    particles.push_back(std::make_shared<Line>(midContainerRight));
    pointsA = new glm::vec2[4];
    pointsA[0] = midContainerA;
    pointsA[1] = midContainerB;
    pointsA[2] = midContainerC;
    pointsA[3] = midContainerD;
    polygonAPoints.clear();
    polygonAPoints.push_back(midContainerA);
    polygonAPoints.push_back(midContainerB);
    polygonAPoints.push_back(midContainerC);
    polygonAPoints.push_back(midContainerD);
    polygonAPos = {13.25f, -3.4f};

    glm::vec2 endContainerA(20.5f, 2.2f);
    glm::vec2 endContainerB(21.0f, 1.0f);
    glm::vec2 endContainerC(24.5f, 1.0f);
    glm::vec2 endContainerD(25.0f, 2.2f);
    Line endContainerLeft = Line(endContainerA, endContainerB);
    Line endContainerRight = Line(endContainerC, endContainerD);
    endContainerLeft.Colour = Colors::yellow;
    endContainerRight.Colour = Colors::yellow;
    particles.push_back(std::make_shared<Line>(endContainerLeft));
    particles.push_back(std::make_shared<Line>(endContainerRight));
    pointsB = new glm::vec2[4];
    pointsB[0] = endContainerA;
    pointsB[1] = endContainerB;
    pointsB[2] = endContainerC;
    pointsB[3] = endContainerD;
    polygonBPoints.clear();
    polygonBPoints.push_back(endContainerA);
    polygonBPoints.push_back(endContainerB);
    polygonBPoints.push_back(endContainerC);
    polygonBPoints.push_back(endContainerD);
    polygonBPos = {22.5f, 1.6f};

    // rectangles
    Rectangle green1 = Rectangle({0.5, 0.5f}, {-8.4f, 0.05f}, {0.0f, 0.0f}, 1.0f,
                               0.5f, 0.5f, 0.0f, 0.0f, 0.0f, false, false,
                               true, false, false, Colors::green);
    Rectangle yellow1 = Rectangle({0.5, 0.5f}, {-7.3f, 0.05f}, {0.0f, 0.0f},
                                 1.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, false,
                                 false, true, false, false, Colors::yellow);
    Rectangle green2 = Rectangle({0.5, 0.5f}, {-6.2f, 0.05f}, {0.0f, 0.0f},
                                 1.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, false,
                                 false, true, false, false, Colors::green);
    Rectangle yellow2 = Rectangle({0.5, 0.5f}, {-5.1f, 0.05f}, {0.0f, 0.0f},
                                  1.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, false,
                                  false, true, false, false, Colors::yellow);
    Rectangle green3 = Rectangle({0.5, 0.5f}, {-4.2f, 1.05f}, {0.0f, 0.0f},
                                 1.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, false,
                                 false, true, false, false, Colors::green);
    Rectangle yellow3 = Rectangle({0.5, 0.5f}, {-6.8f, 1.05f}, {0.0f, 0.0f},
                                  1.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, false,
                                  false, true, false, false, Colors::yellow);
    particles.push_back(std::make_shared<Rectangle>(green1));
    particles.push_back(std::make_shared<Rectangle>(green2));
    particles.push_back(std::make_shared<Rectangle>(green3));
    particles.push_back(std::make_shared<Rectangle>(yellow1));
    particles.push_back(std::make_shared<Rectangle>(yellow2));
    particles.push_back(std::make_shared<Rectangle>(yellow3));

    // magnet
    Rectangle m = Rectangle({0.5, 0.5f}, {3.0f, 5.5f}, {0.0f, 0.0f}, 0.0f,
                               0.5f, 0.5f, 0.0f, 0.0f, 0.0f, false, false,
                               false, true, false, Colors::white);
    m.MagneticFieldStrength = 25.0f;
    magnet = std::make_shared<Rectangle>(m);
    particles.push_back(magnet);
}

glm::vec2 FinalAssignment::CalculateMagneticForce(
    const std::shared_ptr<RigidBody>& magnet,
    const std::shared_ptr<RigidBody>& metal) {
    glm::vec2 direction = magnet->Position - metal->Position;
    float distance = glm::length(direction);
    direction = glm::normalize(direction);

    // Simplified magnetic force (proportional to field strength and
    // inversely proportional to the square of the distance)
    // I tried a lot of different methods that were all AWFUL, this was the best I could find
    float forceMagnitude =
        (magnet->MagneticFieldStrength) / (distance * distance);

    return direction * forceMagnitude;
}
