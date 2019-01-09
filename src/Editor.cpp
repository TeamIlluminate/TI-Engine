#include "Editor.hpp"
#include "GameMaster.h"
#include "Scene.h"
#include "GameObject.h"

using namespace eng;

void Editor::DrawInspector()
{
    auto _window = GameMaster::Get().GetWindow();

    if (auto window = _window.lock())
    {
        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoCollapse;
        flags |= ImGuiWindowFlags_NoMove;
        flags |= ImGuiWindowFlags_NoResize;
        flags |= ImGuiWindowFlags_MenuBar;
        //flags |= ImGuiWindowFlags_Popup;

        ImGui::Begin("Inspector", nullptr, flags);

        ImGui::BeginMenuBar();

        if (ImGui::MenuItem("Play", "bibanatrii", false))
        {
            if (!(GameMaster::Get().state & GameMaster::_GAME) & !(GameMaster::Get().state & GameMaster::_PAUSE))
            {
                GameMaster::Get().state = GameMaster::_DEVELOP + GameMaster::_GAME;
                auto _currScene = GameMaster::Get().GetCurrentScene();
                if (auto currentScene = _currScene.lock())
                {
                    auto gameScene = make_shared<Scene>(*currentScene.get());

                    GameMaster::Get().SaveScene(currentScene);
                    GameMaster::Get().LoadScene(gameScene);

                    gameScene->Rebind();
                }
            }
            else if (GameMaster::Get().state & GameMaster::_PAUSE)
            {
                GameMaster::Get().state = GameMaster::_DEVELOP + GameMaster::_GAME;
            }
        }

        if (ImGui::MenuItem("Pause", "bibanatrii", false))
        {
            if (GameMaster::Get().state & GameMaster::_GAME)
            {
                GameMaster::Get().state = GameMaster::_DEVELOP + GameMaster::_EDITOR + GameMaster::_PAUSE;
            }
        }

        if (ImGui::MenuItem("Stop", "bibanatrii", false))
        {
            if (GameMaster::Get().state & (GameMaster::_GAME | GameMaster::_PAUSE))
            {
                GameMaster::Get().state = GameMaster::_DEVELOP + GameMaster::_EDITOR;
                if (auto editScene = GameMaster::Get().GetEditableScene().lock())
                {
                    GameMaster::Get().LoadScene(editScene);
                }
            }
        }

        ImGui::EndMenuBar();

        ImGui::SetWindowSize(sf::Vector2i(300, window->getSize().y));
        ImGui::SetWindowPos(sf::Vector2i(0, 0));

        auto _c_Scene = GameMaster::Get().GetCurrentScene();

        if (auto c_Scene = _c_Scene.lock())
        {
            auto _gameObjects = c_Scene->GetGameObjects();

            for (auto _gameObject : _gameObjects)
            {
                if (auto gameObject = _gameObject.lock())
                {
                    gameObject->DrawEditor();
                }
            }
        }

        ImGui::End();
    }
}
