#include "../include/FuzzyCubeApp.h"
#include <cstring>

int main(int argc, char* argv[]) {
    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--verbose") == 0 || std::strcmp(argv[i], "-v") == 0) {
            g_verbose = true;
            std::cout << "[INFO] Verbose mode enabled" << std::endl;
        } else if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0) {
            std::cout << "Fuzzy 3D Cube Renderer - Graphics Quality Adaptation Demo\n\n";
            std::cout << "Usage: " << argv[0] << " [OPTIONS]\n\n";
            std::cout << "Options:\n";
            std::cout << "  -v, --verbose    Enable verbose debug output\n";
            std::cout << "  -h, --help       Show this help message\n\n";
            std::cout << "Controls:\n";
            std::cout << "  0  - Auto quality mode (fuzzy logic)\n";
            std::cout << "  1  - Force low quality\n";
            std::cout << "  2  - Force medium quality\n";
            std::cout << "  3  - Force high quality\n";
            std::cout << "  ESC - Exit application\n";
            return 0;
        } else {
            std::cerr << "Unknown option: " << argv[i] << std::endl;
            std::cerr << "Use --help for usage information" << std::endl;
            return 1;
        }
    }
    
    FuzzyCubeApp app;
    
    if (!app.initialize()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }
    
    app.run();
    app.cleanup();
    
    return 0;
}
