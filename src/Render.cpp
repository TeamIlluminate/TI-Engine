#include "Render.h"
#include "Scene.h"
#include "GameMaster.h"

using namespace eng;

//Initialization of the window. If we already have a window -> close current and create new.
Render::Render(sf::VideoMode mode)
{
    //Здесь проблема, займусь, когда проснусь. Или сам поправь -> if(!window) не работает адекватно, быть может нужно инитить nullptr
    if(true)
    {
        this->currentScene = GameMaster::Get().GetCurrentScene();
        this->window = new sf::RenderWindow(mode, "TI Engine improved");
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
        this->windowThread->launch(); //Restart thread
    }
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

        window->display();
    }
}