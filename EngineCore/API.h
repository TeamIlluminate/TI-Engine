#include <iostream>
#include "GameMaster.h"
#include "Render.h"
#include "Scene.h"
#include <Box2D/Box2D.h>
#include "SerializableGenerator.cpp"
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    void printZT()
    {
        eng::Render *render = new eng::Render();
        render->Init(sf::VideoMode::getFullscreenModes()[0]);

        eng::GameMaster::Get().GameStarted(true);
        eng::GameMaster &gameMaster = eng::GameMaster::Get();

        shared_ptr<eng::Scene> loadScene = make_shared<eng::Scene>("emptyScene");
        gameMaster.LoadScene(loadScene);

        delete new eng::SerializableGenerator();

        while (eng::GameMaster::Get().IsGameStarted())
        {
        } //Now imitate game\editor cycle
    };

#ifdef __cplusplus
}
#endif