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
    weak_ptr<sf::RenderWindow> GetWindow();
    void SetWindow();
    void SetWindow(shared_ptr<sf::RenderWindow> window);
    float GetDeltaTime() const;
    string GetGUI_ID() { return "ImID" + to_string(GuiElementCounter); GuiElementCounter++;};
    float UpdateDeltaTime(float dt);

    sf::VideoMode mode;

  private:
    GameMaster(){};
    ~GameMaster(){};
    GameMaster(GameMaster const &) = delete;
    GameMaster &operator=(GameMaster const &) = delete;

    bool gameStatus = false;
    shared_ptr<sf::RenderWindow> window;
    shared_ptr<Scene> currentScene;
    shared_ptr<Scene> editorScene;
    float deltaTime = 0;
    int GuiElementCounter = 0;
};

} // namespace eng