#include "../include/FuzzyCubeApp.h"

int main() {
    FuzzyCubeApp app;
    
    if (!app.initialize()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }
    
    app.run();
    app.cleanup();
    
    return 0;
}
