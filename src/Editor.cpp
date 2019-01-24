#include "Editor.hpp"
#include "GameMaster.h"
#include "Scene.h"
#include "GameObject.h"
#include "Component.h"
#include "Serializable.h"
#include <fstream>
using namespace eng;

void Editor::DrawInspector()
{
    auto editor = GameMaster::Get().GetEditorInst();
    auto _window = GameMaster::Get().GetWindow();

    if (auto window = _window.lock())
    {
        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoCollapse;
        flags |= ImGuiWindowFlags_NoMove;
        flags |= ImGuiWindowFlags_MenuBar;

        ImGui::SetNextWindowSize(sf::Vector2i(300, window->getSize().y), ImGuiCond_Appearing);
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

        if (ImGui::MenuItem("Save", "bibanatrii", false))
        {
            editor->SaveFileDialog("Scenes");
        }

        if (ImGui::MenuItem("Open", "bibanatrii", false))
        {
            editor->OpenFileDialog("Scenes");
        }

        ImGui::EndMenuBar();

        ImGui::SetWindowPos(sf::Vector2i(0, 0));

        GameMaster::Get().GetEditorInst()->UpdateEditor();

        if (editor->SFD_Status())
        {
            if (auto currentScene = GameMaster::Get().GetCurrentScene().lock())
            {
                try
                {
                    auto data = currentScene->Serialize();
                    ofstream save;
                    save.open(*editor->GetSFD_Result() + ".sb");
                    save << data;
                    save.close();
                }
                catch (exception)
                {
                }
            }
        }

        if (editor->OFD_Status())
        {
            ifstream open;
            open.open(*editor->GetOFD_Result());
            string source;
            string line;
            while (getline(open, line))
            {
                source += line;
            }
            open.close();
            auto scene = make_shared<Scene>();
            try
            {
                json data = json::parse(source);
                scene->Deserialize(data);
                GameMaster::Get().LoadScene(scene);
            }
            catch (exception)
            {
            }
        }

        auto _c_Scene = GameMaster::Get().GetCurrentScene();

        if (auto c_Scene = _c_Scene.lock())
        {
            auto _gameObjects = c_Scene->GetGameObjects();

            for (auto _gameObject : _gameObjects)
            {
                if (auto gameObject = _gameObject.lock())
                {
                    gameObject->DrawEditor();
                    gameObject->UpdateEditor();
                }
            }
            ImGui::Separator();
            if (ImGui::Button("Create new GameObject"))
            {
                auto newGameObject = c_Scene->CreateGameObject();
                newGameObject->transform.position = GetCenterScreenCoordinates();
            }
        }

        ImGui::SameLine();

        ImGui::End();
    }
}

void Editor::OpenFileDialog(fs::path path, Component *cmp, string ext)
{
    if (!OFD_open)
    {
        string name;
        string id;

        if (path == "")
            path = fs::current_path();

        if (cmp)
        {
            name = typeid(*cmp).name();
            id = "Choose file:##" + name + "_" + to_string(cmp->_owner.lock()->id);
        }
        else
        {
            name = "Inspector";
            id = "Choose file:##" + name + "_0x00000";
        }

        OFD_Data ofd_data = {id, path, make_shared<string>(), ext};
        OFD_data = ofd_data;

        OFD_open = true;
    }
}

void Editor::SaveFileDialog(fs::path pathStart, Component *cmp)
{
    if (!SFD_open)
    {
        string name;
        string id;

        if (pathStart == "")
            pathStart = fs::current_path();

        if (cmp)
        {
            name = typeid(*cmp).name();
            id = "Choose file:##" + name + "_" + to_string(cmp->_owner.lock()->id);
        }
        else
        {
            name = "Inspector";
            id = "Choose file:##" + name + "_0x00000";
        }
        SFD_Data data = {id, pathStart, make_shared<string>()};
        SFD_data = data;
        SFD_open = true;
    }
}

void Editor::UpdateEditor()
{
    if (OFD_open)
    {
        DrawOpenFileDialog();
    }
    if (SFD_open)
    {
        DrawSaveFileDialog();
    }
}

void Editor::DrawOpenFileDialog()
{
    ImGui::SetNextWindowPosCenter(ImGuiCond_Appearing);
    ImGui::Begin(OFD_data.id.c_str(), nullptr, sf::Vector2i(500, 500));

    if (!fs::exists(OFD_data.path))
        fs::create_directory(OFD_data.path);
    {
        fs::recursive_directory_iterator begin(OFD_data.path);
        fs::recursive_directory_iterator end;

        std::vector<fs::path> subdirs;
        std::copy_if(begin, end, std::back_inserter(subdirs), [](const fs::path &path) {
            return fs::is_directory(path);
        });
    }

    fs::recursive_directory_iterator begin(OFD_data.path);
    fs::recursive_directory_iterator end;

    std::vector<fs::path> files;

    if (OFD_data.extension == "")
    {
        std::copy_if(begin, end, std::back_inserter(files), [](const fs::path &path_) {
            return fs::is_regular_file(path_);
        });
    }
    else
    {
        OFD_Data fd_data = OFD_data;
        std::copy_if(begin, end, std::back_inserter(files), [fd_data](const fs::path &path_) {
            return fs::is_regular_file(path_) && path_.extension() == fd_data.extension;
        });
    }
    static int select = -1;
    int file_iterator = 0;

    for (auto file_ : files)
    {
        if (ImGui::Selectable(file_.c_str(), select == file_iterator))
        {
            select = file_iterator;
        }
        file_iterator++;
    }

    if (ImGui::Button("OK"))
    {
        *OFD_data.output = files[select];
        OFD_open = false;
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel"))
    {
        *OFD_data.output = "";
        OFD_open = false;
    }

    ImGui::End();
}

void Editor::DrawSaveFileDialog()
{
    ImGui::SetNextWindowPosCenter(ImGuiCond_Appearing);
    ImGui::Begin(SFD_data.id.c_str(), nullptr, sf::Vector2i(500, 500));

    if (!fs::exists(SFD_data.path))
        fs::create_directory(SFD_data.path);

    fs::recursive_directory_iterator begin(SFD_data.path);
    fs::recursive_directory_iterator end;

    std::vector<fs::path> subdirs;
    std::copy_if(begin, end, std::back_inserter(subdirs), [](const fs::path &path) {
        return fs::is_directory(path);
    });

    static int select = -1;
    int file_iterator = 0;

    for (auto dir : subdirs)
    {
        if (ImGui::Selectable(dir.c_str(), select == file_iterator))
        {
            select = file_iterator;
        }
        file_iterator++;
    }

    ImGui::InputText("FileName: ", SFD_data.output.get());

    if (ImGui::Button("OK"))
    {
        if(select == -1)
        {   
            string path = SFD_data.path;
            string absPath = path + "/" + *SFD_data.output.get();
            *SFD_data.output = absPath;
            SFD_open = false;
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel"))
    {
        *SFD_data.output = "";
        SFD_open = false;
    }

    ImGui::End();
}
