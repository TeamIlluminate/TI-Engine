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

std::list<weak_ptr<GameObject> > Scene::GetGameObjects() const
{
    list<weak_ptr<GameObject> > weaks;
    for(auto shared : this->sceneObjects)
    {
        weaks.push_back((weak_ptr<GameObject>)shared);
    }

    return weaks;
}

void Scene::AddGameObject(shared_ptr<GameObject> object)
{
    object->SetScene(this);
    this->sceneObjects.push_back(object);
}

weak_ptr<GameObject> Scene::FindGameObject(std::string objectName) const
{
    for (auto gameObject : sceneObjects)
    {
        if (gameObject->GetName() == objectName)
            return gameObject;
    }
}

std::list<weak_ptr<GameObject> > Scene::FindGameObjects(std::string objectName) const
{
    std::list<weak_ptr<GameObject>> findedObjects;

    for (auto gameObject : sceneObjects)
    {
        if (gameObject->GetName() == objectName)
            findedObjects.push_back(gameObject);
    }

    return findedObjects;
}

void Scene::Destroy(weak_ptr<GameObject> gameObject)
{
    sceneObjects.remove((shared_ptr<GameObject>)gameObject);
}

void Scene::PhysicsLoop()
{
    while (GameMaster::Get().IsGameStarted())
    {
        float32 timeStep = 0.02f;

        world->Step(timeStep, velocityIterations, positionIterations);

        for (shared_ptr<GameObject> gameObject : sceneObjects)
        {
            auto components = gameObject->GetComponents();
            for (auto component : components)
            {
                component.lock()->FixedUpdate();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}