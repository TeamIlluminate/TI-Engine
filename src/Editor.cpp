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

        static string s;
        static bool close = false;

        if(!close)
        if(DrawOpenFileDialog("Resource", s))
        {
            if(s != "")
            {
                std::cout << "Choosed file is: " << s << '\n';
                close = true;
            }
        }
    }
}

bool Editor::DrawOpenFileDialog(fs::path path, string &file)
{
    bool exit = false;

    ImGui::Begin("Choose file:");

    if (!fs::exists(path))
        fs::create_directory(path);
    {
        fs::recursive_directory_iterator begin(path);
        fs::recursive_directory_iterator end;

        std::vector<fs::path> subdirs;
        std::copy_if(begin, end, std::back_inserter(subdirs), [](const fs::path &path) {
            return fs::is_directory(path);
        });
    }

    fs::recursive_directory_iterator begin(path);
    fs::recursive_directory_iterator end;

    std::vector<fs::path> files;
    std::copy_if(begin, end, std::back_inserter(files), [](const fs::path &path_) {
        return fs::is_regular_file(path_);
    });

    static int select = -1;
    int file_iterator = 0;

    for (auto file_ : files)
    {
        if (ImGui::Selectable(file_.c_str(), select == file_iterator))
        {
            file = file_;
            select = file_iterator;
        }
        file_iterator++;
    }

    if(ImGui::Button("OK"))
    {
        exit = true;
    }

    if(ImGui::Button("Cancel"))
    {
        file = "";
        exit = true;
    }

    ImGui::End();

    return exit;
}
