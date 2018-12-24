#include "Utils.h"

namespace eng{

class Scene;

    class Render
    {
        public:
        
        Render(sf::VideoMode mode);

        private:

        sf::RenderWindow* window = nullptr;
        sf::Thread* windowThread = nullptr;
        Scene* currentScene = nullptr;

        void WindowLoop();

    };

}