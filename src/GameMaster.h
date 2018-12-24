#include "Utils.h"

namespace eng{

class Scene;

    class GameMaster
    {
        public:

        static GameMaster& Get();

        Scene* GetCurrentScene() const;
        bool IsGameStarted() const;
        void GameStarted();
        void LoadScene(Scene* scene);
        
        sf::VideoMode mode;

        private:
        
        GameMaster(){};
        ~GameMaster(){};
        GameMaster(GameMaster const&) = delete;
        GameMaster& operator= (GameMaster const&) = delete;

        bool gameStatus = false;
        Scene* currentScene;
    };

}