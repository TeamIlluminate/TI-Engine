#include "Utils.h"
#pragma once

namespace eng
{

class Scene;

class GameMaster
{
  public:

    enum STATE {
      _EDITOR = 1,
      _PAUSE = 2,
      _GAME = 4,
      _DEVELOP = 8,
    }; 

    int state = _DEVELOP | _EDITOR;

    static GameMaster &Get();

    weak_ptr<Scene> GetCurrentScene() const;
    weak_ptr<Scene> GetEditableScene() const {return this->editorScene; };

    bool IsGameStarted() const;
    void GameStarted(bool status);
    void SaveScene(shared_ptr<Scene> scene) {this->editorScene = scene; };
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
    shared_ptr<Scene> editorScene;
    float deltaTime = 0;
};

} // namespace eng