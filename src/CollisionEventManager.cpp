#include "CollisionEventManager.h"
#include "Scene.h"
#include "Components/PhysBody.h"

using namespace eng;

CollisionEventManager::CollisionEventManager(Scene * scene) : scene(scene) {}

void CollisionEventManager::BeginContact(b2Contact * contact) {

    b2Body* first = contact->GetFixtureA()->GetBody();
    b2Body* second = contact->GetFixtureB()->GetBody();

    GameObject* firstObject;
    GameObject* secondObject;

    auto gameObjects = scene->GetGameObjects();

    for (GameObject* gameObject : gameObjects) {

        PhysBody* body = gameObject->GetComponent<PhysBody>();

        if (body)
        {   
            if(body->body == first) 
            {
                firstObject = gameObject;
            }
            if(body->body == second)
            {
                secondObject = gameObject;
            }
        }             
    }

    auto components = firstObject->GetComponents();

    for (auto component : components) 
    {
        component->BeginContact(secondObject);
    }

    components = secondObject->GetComponents();

    for (auto component : components) 
    {
        component->BeginContact(firstObject);
    }

}

void CollisionEventManager::EndContact(b2Contact * contact) {
    
    b2Body* first = contact->GetFixtureA()->GetBody();
    b2Body* second = contact->GetFixtureB()->GetBody();

    GameObject* firstObject;
    GameObject* secondObject;

    auto gameObjects = scene->GetGameObjects();

    for (GameObject* gameObject : gameObjects) {

        PhysBody* body = gameObject->GetComponent<PhysBody>();

        if (body)
        {   
            if(body->body == first) 
            {
                firstObject = gameObject;
            }
            if(body->body == second)
            {
                secondObject = gameObject;
            }
        }             
    }

    auto components = firstObject->GetComponents();

    for (auto component : components) 
    {
        component->EndContact(secondObject);
    }

    components = secondObject->GetComponents();

    for (auto component : components) 
    {
        component->EndContact(firstObject);
    }
}