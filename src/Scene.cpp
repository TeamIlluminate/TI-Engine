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
}

b2World *Scene::GetWorld() const
{
    return this->world;
}

void Scene::AddB2BodyToDelete(b2Body* body) {
    toDelete.push_back(body);
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
    //this->sceneObjects.push_front(object);
    this->neededToAdd.push_back(object);
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
        for (auto gameObject : this->neededToAdd) {
            gameObject->SetScene(this);
            auto components = gameObject->GetComponents();
            for (auto refComponent : components) {
                auto concreteComponent = refComponent.lock();
                concreteComponent->OnInit();
            }
            this->sceneObjects.push_back(gameObject);
        }
        neededToAdd.clear();

        float32 timeStep = 0.02f;

        for (auto bodyToDelete : toDelete) {
            world->DestroyBody(bodyToDelete);
        }
        toDelete.clear();

        world->Step(timeStep, velocityIterations, positionIterations);


        for (auto gameObject : sceneObjects)
        {
            auto components = gameObject->GetComponents();
            for (auto component : components)
            {
                if(auto cmp = component.lock())
                {
                    cmp->FixedUpdate();
                }
            }
        }
}