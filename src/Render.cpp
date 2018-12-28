#include "Render.h"
#include "Scene.h"
#include "GameMaster.h"
#include "Transform.h"
#include "Components/Mesh.h"
#include "GameObject.h"
#include "Component.h"
#include "imgui.h"
#include "imgui-SFML.h"

using namespace eng;

//Initialization of the window. If we already have a window -> close current and create new.
Render::Render(sf::VideoMode mode)
{
    if (!this->window)
    {
        this->currentScene = GameMaster::Get().GetCurrentScene();
        this->window = new sf::RenderWindow(mode, "TI Engine improved");
        //window must be deactived in thread which it where created
        this->window->setActive(false);

        this->wThread = new std::thread(&Render::WindowLoop, this);
        this->wThread->detach();
    }
    else
    {
        //Here's REcreate window with new VideoMode(new size also)
        //////////////////////////////////////////////////////////

        //First - free space
        this->wThread->~thread(); // ->terminate(); //Terminate window drawning loop;
        delete this->window;      //Free space 4r new window

        this->window = new sf::RenderWindow(mode, "TI Engine improved"); //I wanna make some spec file-descriptor like (Project name; Engine version; ... etc)
        this->window->setActive(false);

        this->wThread = new std::thread(&Render::WindowLoop, this); //Restart thread
        this->wThread->detach();
    }
}

void Render::SetScene(Scene *scene)
{
    this->currentScene = scene;
}

void Render::WindowLoop()
{
    window->clear();

    ImGui::SFML::Init(*window);

    sf::Clock deltaClock;

    while (window->isOpen())
    {

        window->clear();
        sf::Clock DeltaClock;
        sf::Event event;

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

        if (currentScene)
        {
            auto references = this->currentScene->GetGameObjects();

            for (auto reference : references)
            {
                if (auto concreteObject = reference.lock()) {
                    Draw(concreteObject);
                }
            }
        }

        ImGui::SFML::Render(*window);

        GameMaster::Get().UpdateDeltaTime(deltaClock.restart().asSeconds());

        window->display();
    }
    ImGui::SFML::Shutdown();
}

void Render::Draw(shared_ptr<GameObject> object)
{
    auto references = object->GetComponents();

    for (auto reference : references)
    {
        if (auto component = reference.lock()) {
            component->Update();
            component->GUI();
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