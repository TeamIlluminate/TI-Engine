#include "Render.h"
#include "Scene.h"
#include "GameMaster.h"
#include "Transform.h"
#include "Components/Mesh.h"
#include "GameObject.h"
#include "Component.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Components/Camera.h"

using namespace eng;

//Initialization of the window. If we already have a window -> close current and create new.
Render::Render(sf::VideoMode mode)
{
    if (!this->window)
    {
        this->window = make_shared<sf::RenderWindow>(mode, "TI Engine improved");
        //window must be deactived in thread which it where created
        this->window->setActive(false);

        this->wThread = make_shared<std::thread>(&Render::WindowLoop, this);
        this->wThread->detach();
    }
    else
    {
        //Here's REcreate window with new VideoMode(new size also)
        //////////////////////////////////////////////////////////

        //First - free space
        this->wThread->~thread(); // ->terminate(); //Terminate window drawning loop;

        this->window =  make_shared<sf::RenderWindow>(mode, "TI Engine improved"); //I wanna make some spec file-descriptor like (Project name; Engine version; ... etc)
        this->window->setActive(false);

        this->wThread = make_shared<std::thread>(&Render::WindowLoop, this); //Restart thread
        this->wThread->detach();
    }
}

void Render::WindowLoop()
{
    window->clear();

    ImGui::SFML::Init(*window);

    sf::Clock deltaClock;
    float fixedDelta = 0;

    while (window->isOpen())
    {

        window->clear();
        sf::Clock DeltaClock;
        sf::Event event;

        float dTime = deltaClock.restart().asSeconds();

        auto currentScene = GameMaster::Get().GetCurrentScene().lock();

        if(fixedDelta < 0.02f)
        fixedDelta += dTime;
        else
        {
            if(currentScene && GameMaster::Get().state & GameMaster::_GAME)
            currentScene->PhysicsLoop();
            
            fixedDelta = 0;
        }

        GameMaster::Get().UpdateDeltaTime(dTime);

        while (window->pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
            {
                window->close();
                GameMaster::Get().GameStarted(false);
            }
        }

        if (currentScene)
        {
            ImGui::SFML::Update(*window, deltaClock.restart());

            auto meshes = currentScene->Update();
            for (auto _mesh : meshes)
            {
                if (auto mesh = _mesh.lock()) {            
                    if(auto drawable = mesh->GetDrawable().lock())
                    window->draw(*drawable.get(), mesh->GetRenderStates());
                }
            }
        

            auto references = currentScene->GetGameObjects();

            if(GameMaster::Get().state & GameMaster::_DEVELOP)
            {
                ImGuiWindowFlags flags = 0;
                flags |= ImGuiWindowFlags_NoCollapse;
                flags |= ImGuiWindowFlags_NoMove;
                flags |= ImGuiWindowFlags_NoResize;

                ImGui::Begin("Inspector", nullptr , flags);

                ImGui::SetWindowSize(sf::Vector2i(300, window->getSize().y));
                ImGui::SetWindowPos(sf::Vector2i(0,0));

                for(auto reference : references)
                {
                    if(auto _gameObject = reference.lock())
                    {
                        _gameObject->DrawEditor();
                    }
                }

                ImGui::End();

                ImGui::Begin("Game State"); 

                if(ImGui::Button("Play"))
                {
                    //Если игровой цикл не запущен.
                    if(!(GameMaster::Get().state & GameMaster::_GAME) & !(GameMaster::Get().state & GameMaster::_PAUSE))
                    {
                        GameMaster::Get().state = GameMaster::_DEVELOP + GameMaster::_GAME;
                        auto gameScene = make_shared<Scene>(*currentScene.get());
                        GameMaster::Get().SaveScene(currentScene);
                        GameMaster::Get().LoadScene(gameScene);
                        gameScene->ResetScene();
                    }
                    else if(GameMaster::Get().state & GameMaster::_PAUSE)
                    {
                        GameMaster::Get().state = GameMaster::_DEVELOP + GameMaster::_GAME;                       
                    }
                }

                if(ImGui::Button("Pause"))
                {
                    if(GameMaster::Get().state & GameMaster::_GAME)
                    {
                        GameMaster::Get().state = GameMaster::_DEVELOP + GameMaster::_EDITOR + GameMaster::_PAUSE;             
                    }                 
                }

                if(ImGui::Button("Stop"))
                {
                    if(GameMaster::Get().state & (GameMaster::_GAME | GameMaster::_PAUSE))
                    {
                        GameMaster::Get().state = GameMaster::_DEVELOP + GameMaster::_EDITOR; 
                        if(auto editScene = GameMaster::Get().GetEditableScene().lock())
                        GameMaster::Get().LoadScene(editScene);
                    }                 
                }

                ImGui::End();                
            }


        ImGui::SFML::Render(*window);
        auto cameras = currentScene->GetCameras();
        for(auto _camera : cameras)
        {
            if(auto camera = _camera.lock())
            {
                if(camera->isEnabled)
                window->setView(camera->view);
            }
        }

        window->display();
        }
    }
    ImGui::SFML::Shutdown();
}