#include "Utils.h"

namespace eng{

class Scene;
class GameObject;

    class Render
    {
        public:
        
        Render(sf::VideoMode mode);

        void SetScene(Scene* scene);

        private:

        sf::RenderWindow* window = nullptr;
        sf::Thread* windowThread = nullptr;
        Scene* currentScene = nullptr;

        void WindowLoop();
        void Draw(GameObject* object);
    };

}