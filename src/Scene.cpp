#include "Scene.h"
#include "GameObject.h"
#include "GameMaster.h"
#include "Components/Camera.h"

using namespace eng;

Scene::Scene(std::string name)
{
    this->name = name;

    b2Vec2 gravity;
    world = make_shared<b2World>(gravity);

    collisionEventManager = new CollisionEventManager(this);
    world->SetContactListener(collisionEventManager);
}

weak_ptr<b2World> Scene::GetWorld() const
{
    return this->world;
}

void Scene::AddB2BodyToDelete(b2Body* body) {
    toDelete.push_back(body);
}

std::list<weak_ptr<GameObject> > Scene::GetGameObjects() const
{
    list<weak_ptr<GameObject> > _gameObjects;
    for(auto gameObject : this->sceneObjects)
    {
        _gameObjects.push_back(gameObject);
    }
    return _gameObjects;
}

void Scene::AddGameObject(shared_ptr<GameObject> object)
{
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
    std::list<weak_ptr<GameObject>> _foundedObjects;

    for (auto gameObject : sceneObjects)
    {
        if (gameObject->GetName() == objectName)
            _foundedObjects.push_back(gameObject);
    }

    return _foundedObjects;
}

void Scene::Destroy(weak_ptr<GameObject> _gameObject)
{
    auto gameObject = _gameObject.lock();
    sceneObjects.remove(gameObject);
}

void Scene::PhysicsLoop()
{
        PushGameobjects();

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

void Scene::PushGameobjects() {
     for (auto gameObject : this->neededToAdd) {
            gameObject->SetScene(shared_from_this());
            auto components = gameObject->GetComponents();
            for (auto refComponent : components) {
                auto concreteComponent = refComponent.lock();
                concreteComponent->OnInit();
            }
            auto camera = gameObject->GetComponent<Camera>();
            if (auto concreteCamera = camera.lock()) {
                cameras.push_back(concreteCamera);
            }
            this->sceneObjects.push_back(gameObject);
            gameObject->id = idCounter;
            idCounter++;
        }
        neededToAdd.clear();
}