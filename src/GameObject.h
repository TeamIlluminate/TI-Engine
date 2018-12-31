#include "Utils.h"
#include "Transform.h"
#include "Scene.h"
#pragma once

namespace eng
{

class Component;

class GameObject : public enable_shared_from_this<GameObject>
{
  public:
    GameObject() : name("empty") {};
    GameObject(std::string name) : name(name) {};

    //push reference to child in list and set reference on this gameobject in child
    //delete reference on this child from previous parent of child
    void AddChild(shared_ptr<GameObject> childObject);

    //remove ONLY from list, not from free memory
    //please dont use that
    void RemoveChild(GameObject *child);
    
    //return summary of this gameobject position and him parents gameobject or just this gameobject position if hasnt parent
    sf::Vector2f GetGlobalCoordinates() const;

    std::list<weak_ptr<GameObject> > GetChilds() const;

    weak_ptr<GameObject> GetParent() const;

    //Add component to components list and set to him this gameobject as parent
    void AddComponent(shared_ptr<Component> component);

    template <typename T, typename = enable_if<is_base_of<Component, T>::value>>
    weak_ptr<T> GetComponent()
    {
         for (auto component : this->components)
         {
            shared_ptr<T> requeredComponent = dynamic_pointer_cast<T>(component);
            
            if(requeredComponent) return requeredComponent;
         }
         weak_ptr<T> nullweak;
         return nullweak;
    }

    std::list<weak_ptr<Component> > GetComponents() const;

    //Dont user per now
    void RemoveComponent(Component *component);
    
    const std::string GetName() const;

    void SetName(const std::string newName);

    Scene *GetScene() const;
    //Called when gameobject added to scene
    void SetScene(Scene *scene);
    
    //Coordinates on scene OR coordinates on scene relative to parent (parent is 0 0 point)
    Transform transform = Transform(sf::Vector2f(0, 0));
    int id = 0;
    
  protected:
    Scene *scene = nullptr;

    weak_ptr<GameObject> parent;

    std::string name;
    std::list<shared_ptr<GameObject> > childs;
    std::list<shared_ptr<Component> > components;

};
} // namespace eng