#include "Utils.h"
#include "Transform.h"
#include "Scene.h"
#pragma once
namespace eng
{

class Component;

class GameObject
{
  public:
    GameObject(const std::string name = "empty");
    ~GameObject();
    //push reference to child in list and set reference on this gameobject in child
    //delete reference on this child from previous parent of child
    void AddChild(GameObject *child);
    //remove ONLY from list, not from free memory
    void RemoveChild(GameObject *child);
    //return summary of this gameobject position and him parents gameobject or just this gameobject position if hasnt parent
    sf::Vector2f GetGlobalCoordinates() const;
    std::list<GameObject *> GetChilds() const;
    GameObject *GetParent() const;

    //Add component to components list and set to him this gameobject as parent
    void AddNewComponent(Component *component);
    std::list<Component *> GetComponents() const;
    template <class T>
    T *GetComponent()
    {
        for (Component *component : this->components)
        {
            T *requeredComponent = dynamic_cast<T *>(component);
            if (requeredComponent)
                return requeredComponent;
        }
        return nullptr;
    }
    //remove ONLY from list, not from free memory
    void RemoveComponent(Component *component);
    const std::string GetName() const;
    void SetName(const std::string newName);


    Scene* GetScene() const;
    //Called when gameobject added to scene
    void SetScene(Scene* scene);


    //Coordinates on scene OR coordinates on scene relative to parent (parent is 0 0 point)
    Transform transform = Transform(sf::Vector2f(0, 0));
  protected:
    Scene * scene = nullptr;
    GameObject *parent = nullptr;
    std::string name;
    std::list<GameObject *> childs;
    std::list<Component *> components;
};
} // namespace eng