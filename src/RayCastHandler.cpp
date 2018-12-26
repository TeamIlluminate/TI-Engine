#include "RayCastHandler.h"
#include "GameMaster.h"
#include "Components/PhysBody.h"
#include "Scene.h"

using namespace eng;

float32 RayCastHandler::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction)
{

    b2Body *foundedBody = fixture->GetBody();

    auto gameObjects = GameMaster::Get().GetCurrentScene()->GetGameObjects();
    for (auto gameObject : gameObjects)
    {
        auto physBody = gameObject->GetComponent<PhysBody>();
        if (foundedBody == physBody->body)
        {
            foundedObject = gameObject;
            return 0;
        }
    }
}