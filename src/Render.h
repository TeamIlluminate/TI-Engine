#include "Utils.h"

namespace eng{

class Scene;

    class Render
    {
        public:

        static Render& Get();
        
        void Init(sf::VideoMode mode);
        void WindowLoop();

        private:

        Render(){};
        ~Render(){};

        Render(Render const &) = delete; 
        Render& operator = (Render const&) = delete;

        sf::RenderWindow* window;
        sf::Thread* windowThread;
        Scene* currentScene;

    };

}