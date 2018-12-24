#include "Utils.h"

namespace eng{

class Scene;

    class Render
    {
        public:
        
        Render(sf::VideoMode mode);

        private:

        sf::RenderWindow* window;
        sf::Thread* windowThread;
        Scene* currentScene;

        void WindowLoop();

    };

}