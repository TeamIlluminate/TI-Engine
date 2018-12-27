#include "Scene.h"
#include "GameObject.h"
#include "GameMaster.h"
#include "Components/Camera.h"

using namespace eng;

Scene::Scene(std::string name)
{
    this->name = name;

    b2Vec2 gravity;
    world = new b2World(gravity);

    collisionEventManager = new CollisionEventManager(this);
    (*world).SetContactListener(collisionEventManager);
    physicThread = new std::thread(&Scene::PhysicsLoop, this);
    physicThread->detach();
}

b2World *Scene::GetWorld() const
{
    return this->world;
}

std::list<GameObject *> Scene::GetGameObjects() const
{
    return this->sceneObjects;
}

void Scene::AddGameObject(GameObject *object)
{
    object->SetScene(this);
    this->sceneObjects.push_back(object);
}

GameObject *Scene::FindGameObject(std::string objectName) const
{
    for (GameObject *gameObject : sceneObjects)
    {
        if (gameObject->GetName() == objectName)
            return gameObject;
    }

    return nullptr;
}

std::list<GameObject *> Scene::FindGameObjects(std::string objectName) const
{
    std::list<GameObject *> findedObjects;

    for (GameObject *gameObject : sceneObjects)
    {
        if (gameObject->GetName() == objectName)
            findedObjects.push_back(gameObject);
    }

    return findedObjects;
}

void Scene::RemoveFromScene(GameObject *gameObject)
{
    sceneObjects.remove(gameObject);
}

void Scene::PhysicsLoop()
{
    while (GameMaster::Get().IsGameStarted())
    {
        float32 timeStep = 0.02f;
        world->Step(timeStep, velocityIterations, positionIterations);
        for (GameObject *gameObject : sceneObjects)
        {
            auto components = gameObject->GetComponents();
            for (Component *component : components)
            {
                component->FixedUpdate();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}