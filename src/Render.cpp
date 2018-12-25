#include "Render.h"
#include "Scene.h"
#include "GameMaster.h"
#include "Transform.h"
#include "Components/Mesh.h"
#include "GameObject.h"
#include "Component.h"

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
        this->windowThread = new sf::Thread(&Render::WindowLoop, this);
        this->windowThread->launch();
    }
    else
    {
        //Here's REcreate window with new VideoMode(new size also) 
        //////////////////////////////////////////////////////////

        this->windowThread->terminate(); //Terminate window drawning loop;
        delete this->window; //Free space 4r new window
        this->window = new sf::RenderWindow(mode, "TI Engine improved"); //I wanna make some spec file-descriptor like (Project name; Engine version; ... etc)
        this->window->setActive(false);
        this->windowThread->launch(); //Restart thread
    }
}

void Render::SetScene(Scene* scene)
{
    this->currentScene = scene;
}

void Render::WindowLoop()
{
    window->clear();

    while (window->isOpen())
    {
        sf::Clock DeltaClock;
        sf::Event event;
        
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
                GameMaster::Get().GameStarted(false);
            }
        }

        window->clear();

        if(currentScene)
        {
            std::list<GameObject* > objects = this->currentScene->GetGameObjects();

            for(GameObject* object : objects)
            {
                Draw(object);
            }
        }
        window->display();
    }
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