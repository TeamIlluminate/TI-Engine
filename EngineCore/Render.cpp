#include "Render.h"
#include "Scene.h"
#include "GameMaster.h"
#include "Transform.h"
#include "Components/Mesh.h"
#include "GameObject.h"
#include "Component.h"
#include "Components/Camera.h"
#include "GUI/Grid.h"

using namespace eng;

void Render::Init(sf::VideoMode mode)
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
        this->wThread->~thread();                                                 // ->terminate(); //Terminate window drawning loop;
        this->window = make_shared<sf::RenderWindow>(mode, "TI Engine improved"); //I wanna make some spec file-descriptor like (Project name; Engine version; ... etc)
        this->window->setActive(false);
        this->wThread = make_shared<std::thread>(&Render::WindowLoop, this); //Restart thread
        this->wThread->detach();
    }
}

void Render::WindowLoop()
{
    GameMaster::Get().SetWindow(window);
    window->clear();
    ImGui::SFML::Init(*window);
    defaultView.setSize(sf::Vector2f(window->getDefaultView().getSize()));
    sf::Clock deltaClock;
    sf::Clock imGuiClock;
    float fixedDelta = 0;
    while (window->isOpen())
    {
        window->clear();
        sf::Event event;

        float dTime = deltaClock.restart().asSeconds();
        GameMaster::Get().UpdateDeltaTime(dTime);

        auto currentScene = GameMaster::Get().GetCurrentScene().lock();

        if (fixedDelta < 0.02f)
            fixedDelta += dTime;
        else
        {
            if (currentScene && GameMaster::Get().state & GameMaster::_GAME)
                currentScene->PhysicsLoop();
            fixedDelta = 0;
        }

        while (window->pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
            {
                window->close();
                GameMaster::Get().SetWindow();
                GameMaster::Get().GameStarted(false);
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    defaultView.move(0, -10);
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    defaultView.move(0, 10);
                }
                else if (event.key.code == sf::Keyboard::Left)
                {
                    defaultView.move(-10, 0);
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    defaultView.move(10, 0);
                }
            }
        }

        if (currentScene)
        {
            ImGui::SFML::Update(*window, imGuiClock.restart());

            if (GameMaster::Get().state & GameMaster::_DEVELOP)
            {
                Editor::DrawInspector();
                for(auto grid : GameMaster::Get().GetGrids())
                {
                    grid->Draw();
                }
            }

            auto meshes = currentScene->Update();

            if (GameMaster::Get().state & GameMaster::_GAME)
            {
                auto cameras = currentScene->GetCameras();
                for (auto _camera : cameras)
                {
                    if (auto camera = _camera.lock())
                    {
                        if (camera->isEnabled)
                        {
                            defaultView.setCenter(sf::Vector2f(camera->view.getCenter()));
                            window->setView(camera->view);
                        }
                    }
                }
            }
            else
                window->setView(defaultView);

            for (auto _mesh : meshes)
            {
                if (auto mesh = _mesh.lock())
                {
                    if (auto drawable = mesh->GetDrawable().lock())
                        window->draw(*drawable.get(), mesh->GetRenderStates());
                }
            }

            ImGui::SFML::Render(*window);
            window->display();
        }
    }
    ImGui::SFML::Shutdown();
}