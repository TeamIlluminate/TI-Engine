#include "Scene.h"
#include "GameObject.h"
#include "GameMaster.h"
#include "Components/Camera.h"
#include <Components/Mesh.h>

using namespace eng;

Scene::Scene(std::string name)
{
    this->name = name;
    
    world = make_shared<b2World>(b2Vec2(0,0));
    collisionEventManager = new CollisionEventManager(this);
    world->SetContactListener(collisionEventManager);
}

 Scene::Scene(const Scene &scene)
 {
     this->name = scene.name;
     this->idCounter = scene.idCounter;
     this->world = make_shared<b2World>(b2Vec2(0,0));
     this->collisionEventManager = new CollisionEventManager(this);
     this->world->SetContactListener(this->collisionEventManager);
     auto _sceneObjects = scene.sceneObjects;
     for(auto sceneObject : _sceneObjects  ) {
         this->sceneObjects.push_back(make_shared<GameObject>(*sceneObject.get()));
     }
     auto cameras = scene.cameras;
     this->cameras = cameras;
 }

weak_ptr<b2World> Scene::GetWorld() const
{
    return this->world;
}

void Scene::AddB2BodyToDelete(b2Body *body)
{
    toDelete.push_back(body);
}

std::list<weak_ptr<GameObject>> Scene::GetGameObjects() const
{
    list<weak_ptr<GameObject>> _gameObjects;
    for (auto gameObject : this->sceneObjects)
    {
        _gameObjects.push_back(gameObject);
    }
    return _gameObjects;
}

void Scene::AddGameObject(shared_ptr<GameObject> object)
{
    Initialize(object);
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

std::list<weak_ptr<GameObject>> Scene::FindGameObjects(std::string objectName) const
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

    for (auto gameObject : this->neededToAdd)
    {
        this->sceneObjects.push_back(gameObject);
    }
    neededToAdd.clear();

    float32 timeStep = 0.02f;

    for (auto bodyToDelete : toDelete)
    {
        world->DestroyBody(bodyToDelete);
    }
    toDelete.clear();

    world->Step(timeStep, velocityIterations, positionIterations);

    for (auto gameObject : sceneObjects)
    {
        auto components = gameObject->GetComponents();
        for (auto component : components)
        {
            if (auto cmp = component.lock())
            {
                cmp->FixedUpdate();
            }
        }
    }
}

void Scene::Initialize(shared_ptr<GameObject> gameObject) {
        gameObject->SetScene(shared_from_this());
        auto components = gameObject->GetComponents();
        for (auto refComponent : components)
        {
            auto concreteComponent = refComponent.lock();
            concreteComponent->SetOwner(gameObject);
            concreteComponent->OnInit();
        }
        auto camera = gameObject->GetComponent<Camera>();
        if (auto concreteCamera = camera.lock())
        {
            cameras.push_back(concreteCamera);
        }
        gameObject->id = idCounter;
        idCounter++;
}

void Scene::ForceGameObject(shared_ptr<GameObject> gameObject)
{
    Initialize(gameObject);
    sceneObjects.push_back(gameObject);
}

void Scene::ResetScene()
{
    idCounter = 0;
    for (auto gameObject : sceneObjects)
    {
        Initialize(gameObject);
    }
}

list<weak_ptr<Mesh>> Scene::Update()
{

    list<weak_ptr<Mesh>> meshs;
    auto gameObjects = sceneObjects;

    for (auto gameObject : gameObjects)
    {

        auto list = Update(gameObject);
        meshs.insert(meshs.end(), list.begin(), list.end());
    }

    return meshs;
}

list<weak_ptr<Mesh>> Scene::Update(shared_ptr<GameObject> gameObject)
{

    if (GameMaster::Get().state & GameMaster::_GAME)
    {
        auto components = gameObject->GetComponents();
        for (auto _component : components)
        {
            if (auto component = _component.lock())
            {
                component->Update();
                component->GUI();
            }
        }
    }

    auto childs = gameObject->GetChilds();
    list<weak_ptr<Mesh>> childMeshs;
    for (auto _child : childs)
    {
        auto list = Update(_child.lock());
        childMeshs.insert(childMeshs.end(), list.begin(), list.end());
    }

    auto _mesh = gameObject->GetComponent<Mesh>();
    childMeshs.push_front(_mesh);
    return childMeshs;
}