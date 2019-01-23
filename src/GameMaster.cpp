#include "GameMaster.h"
#include "Scene.h"

using namespace eng;

GameMaster &GameMaster::Get()
{
    static GameMaster instance;
    return instance;
}

void GameMaster::GameStarted(bool status)
{
    this->gameStatus = status;
}

weak_ptr<Scene> GameMaster::GetCurrentScene() const
{
    return this->currentScene;
}

bool GameMaster::IsGameStarted() const
{
    return this->gameStatus;
}

void GameMaster::LoadScene(shared_ptr<Scene> scene)
{
    this->currentScene = scene;
}

float GameMaster::GetDeltaTime() const
{
    return deltaTime;
}

float GameMaster::UpdateDeltaTime(float dt)
{
    deltaTime = dt;
}

//WINDOW

weak_ptr<sf::RenderWindow> GameMaster::GetWindow()
{
    return window;
}

void GameMaster::SetWindow(shared_ptr<sf::RenderWindow> newWindow)
{
    window = newWindow;
}

void GameMaster::SetWindow()
{
    window = nullptr;
}

shared_ptr<Component> GameMaster::GetComponent(json component)
{
    return constructorStorage[component["type"]]();
}

bool GameMaster::VerifyRegister(string name)
{
    if (constructorStorage[name])
        return false;
    else
        return true;
}
void GameMaster::RegisterComponentConstructor(string typeName, shared_ptr<Component> (*cc)())
{
    //if(VerifyRegister(typeName)) 
    constructorStorage.insert(pair<string, shared_ptr<Component> (*)()>(typeName, *cc));
}

string GameMaster::GetStorageNames()
{
    static string storageNames;

    if(storageNames.empty())
    {
        storageNames = "";
        auto allTypes = constructorStorage;
        for(auto typePair : allTypes)
        {
            storageNames = storageNames + typePair.first + '\0';
        }
    }

    return storageNames;
}