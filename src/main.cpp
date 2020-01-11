#include "Application.h"

// main() signature must be this one, otherwise SDL2 will fail to link
int main(int argc, char** argv)
{
    Application app(argc, argv);
    return app.run();
}
