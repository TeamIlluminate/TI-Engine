#include "RayCastHandler.h"
#include "GameMaster.h"
#include "Components/Mesh.h"
#include "Scene.h"

using namespace eng;

float32 RayCastHandler::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction)
{

    b2Body *foundedBody = fixture->GetBody(); 
    auto gameObjects = GameMaster::Get().GetCurrentScene().lock()->GetGameObjects();
    for (auto gameObject : gameObjects)
    {
        auto physBody = gameObject.lock()->GetComponent<Mesh>();
        if (foundedBody == physBody.lock()->GetBody())
        {
            foundedObject = gameObject;
            return 0;
        }
    }
}