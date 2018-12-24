#include "GameMaster.h"
#include "Scene.h"

using namespace eng;

GameMaster& GameMaster::Get()
{
    static GameMaster instance;
    return instance;
}

void GameMaster::GameStarted()
{
    this->gameStatus = true;
}

Scene* GameMaster::GetCurrentScene() const
{
    return this->currentScene;
}

bool GameMaster::IsGameStarted() const
{
    return this->gameStatus;
}

void GameMaster::LoadScene(Scene* scene)
{ 
    this->currentScene = scene;
}
