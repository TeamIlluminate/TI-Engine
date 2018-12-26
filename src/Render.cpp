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
    if(!this->window)
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
        this->wThread->~thread();// ->terminate(); //Terminate window drawning loop;
        delete this->window; //Free space 4r new window

        this->window = new sf::RenderWindow(mode, "TI Engine improved"); //I wanna make some spec file-descriptor like (Project name; Engine version; ... etc)
        this->window->setActive(false);
        
        this->wThread = new std::thread(&Render::WindowLoop, this); //Restart thread
        this->wThread->detach(); 
    }
}

void Render::SetScene(Scene* scene)
{
    this->currentScene = scene;
}

void Render::WindowLoop()
{
    window->clear();

    ImGui::SFML::Init(*window);

    sf::Color bgColor;
    float color[3] = { 0.f, 0.f, 0.f };
    char windowTitle[255] = "ImGui + SFML = <3";
    sf::Clock deltaClock;

    while (window->isOpen())
    {

        window->clear(bgColor);
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

        if(currentScene)
        {
            std::list<GameObject* > objects = this->currentScene->GetGameObjects();

            for(GameObject* object : objects)
            {
                Draw(object);
            }
        }

        ImGui::SFML::Update(*window, deltaClock.restart());        
   
        ImGui::Begin("Sample window");
        
        // Инструмент выбора цвета
        if (ImGui::ColorEdit3("Background color", color)) {
            // код вызывается при изменении значения, поэтому всё
            // обновляется автоматически

            bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
            bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
            bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
        }

        ImGui::InputText("Window title", windowTitle, 255);
        ImGui::ShowDemoWindow();
        if (ImGui::Button("Update window title")) {
            // этот код выполняется, когда юзер жмёт на кнопку
            // здесь можно было бы написать 
            // if(ImGui::InputText(...))
            window->setTitle(windowTitle);
        }

        ImGui::End(); // end window

        ImGui::SFML::Render(*window);

        window->display();
    }
        ImGui::SFML::Shutdown();
}

void Render::Draw(GameObject* object)
{
    std::list<Component*> cmps = object->GetComponents();

    for(Component* cmp : cmps)
    {
        cmp->Update();
    }

    Mesh* mesh = object->GetComponent<Mesh>();

    if(mesh)
    window->draw(*mesh->GetDrawable(), mesh->GetRenderStates());

    //if(object->GetName() == "BB")
    //std::cout << "RenderUpdate: " << object->transform.position.x << " || " << object->transform.position.y << '\n';

    std::list<GameObject* > subObjects = object->GetChilds();

    if(subObjects.size() > 0)
    {
        for(GameObject* subObject : subObjects)
        {
            Draw(subObject);
        }
    }
}