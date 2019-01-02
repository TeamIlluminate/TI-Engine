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
        this->SetScene(GameMaster::Get().GetCurrentScene());
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

void Render::SetScene(weak_ptr<Scene> scene)
{
    this->_currentScene = scene;
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

        if(fixedDelta < 0.02f)
        fixedDelta += dTime;
        else
        {
            if (auto currentScene = _currentScene.lock()) {
            currentScene->PushGameobjects();
            if(currentScene && !isEditor)
            currentScene->PhysicsLoop();
            fixedDelta = 0;
            }
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

        ImGui::SFML::Update(*window, deltaClock.restart());

        if (auto currentScene = _currentScene.lock())
        {

            auto references = currentScene->GetGameObjects();

            if(isEditor)
            {
                ImGui::Begin("Inspector");

                for(auto reference : references)
                {
                    if(auto _gameObject = reference.lock())
                    {
                        _gameObject->DrawEditor();
                    }
                }

                ImGui::End();     
            }

            for (auto reference : references)
            {
                if (auto concreteObject = reference.lock()) {
                    Draw(concreteObject);
                }
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

void Render::Draw(shared_ptr<GameObject> object)
{
    auto references = object->GetComponents();

    if(!isEditor)
    {
        for (auto reference : references)
        {
            if (auto component = reference.lock()) {
                component->Update();
                component->GUI();
            }
        }
    }

    auto meshref = object->GetComponent<Mesh>();

    if (auto mesh = meshref.lock()) {
        auto drawable = mesh->GetDrawable().lock();
        auto renderStates = mesh->GetRenderStates();

        if (drawable) {
            window->draw(*drawable.get(), renderStates);
        }
    }

    auto subObjects = object->GetChilds();

        for (auto subObject : subObjects)
        {
            if (auto concreteObject = subObject.lock()) {
                Draw(concreteObject);
            }
        }
}