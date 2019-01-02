#include "Utils.h"
#pragma once

namespace eng
{

class Scene;

class GameMaster
{
  public:
    static GameMaster &Get();

    weak_ptr<Scene> GetCurrentScene() const;
    bool IsGameStarted() const;
    void GameStarted(bool status);
    void LoadScene(shared_ptr<Scene> scene);
    float GetDeltaTime() const;

    float UpdateDeltaTime(float dt);

    sf::VideoMode mode;

  private:
    GameMaster(){};
    ~GameMaster(){};
    GameMaster(GameMaster const &) = delete;
    GameMaster &operator=(GameMaster const &) = delete;

    bool gameStatus = false;
    shared_ptr<Scene> currentScene;
    float deltaTime = 0;
};

} // namespace eng