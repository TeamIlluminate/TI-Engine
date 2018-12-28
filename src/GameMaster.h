#include "Utils.h"
#pragma once

namespace eng
{

class Scene;

class GameMaster
{
  public:
    static GameMaster &Get();

    Scene *GetCurrentScene() const;
    bool IsGameStarted() const;
    void GameStarted(bool status);
    void LoadScene(Scene *scene);
    float GetDeltaTime() const;

    float UpdateDeltaTime(float dt);

    sf::VideoMode mode;

  private:
    GameMaster(){};
    ~GameMaster(){};
    GameMaster(GameMaster const &) = delete;
    GameMaster &operator=(GameMaster const &) = delete;

    bool gameStatus = false;
    Scene *currentScene = nullptr;
    float deltaTime = 0;
};

} // namespace eng