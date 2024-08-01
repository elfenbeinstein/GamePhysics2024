#include "core/Application.h"
#include "scenes/Assignment1.h"
#include "scenes/Assignment2.h"
#include "scenes/Assignment3.h"
#include "scenes/Assignment4.h"
#include "scenes/Assignment5.h"
#include "scenes/Assignment6.h"
#include "scenes/Assignment7.h"
#include "scenes/Assignment8.h"
#include "scenes/Assignment9.h"

int main(int argc, char** argv) {
    // Add newly created scenes here so that they show up in the menu.
    std::vector<Scene*> scenes = {new Assignment1(), new Assignment2(),
                                  new Assignment3(), new Assignment4(),
                                  new Assignment5(), new Assignment6(),
                                  new Assignment7(), new Assignment8(), 
                                  new Assignment9()};
    Application app(scenes);
    return app.Run();
}
