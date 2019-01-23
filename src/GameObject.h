#include "Utils.h"
#include "Serializable.h"
#include "Transform.h"
#include "Editor.hpp"
#include "Scene.h"
#pragma once

namespace eng
{

class Component;

class GameObject final : public enable_shared_from_this<GameObject>, public Editor, public Serializable
{
public:
  friend class Scene;

  GameObject();
  GameObject(const GameObject &gameObject);
  GameObject(std::string name);
  GameObject(weak_ptr<Scene> scene, std::string name);

  //push reference to child in list and set reference on this gameobject in child
  //delete reference on this child from previous parent of child
  void AddChild(shared_ptr<GameObject> childObject);

  //remove ONLY from list, not from free memory
  //please dont use that
  void RemoveChild(GameObject *child);

  //return summary of this gameobject position and him parents gameobject or just this gameobject position if hasnt parent
  sf::Vector2f GetGlobalCoordinates() const;

  std::list<weak_ptr<GameObject>> GetChilds() const;

  json Serialize();
  void Deserialize(json obj);
  weak_ptr<GameObject> GetParent() const;

  //Add component to components list and set to him this gameobject as parent
  void AddComponent(shared_ptr<Component> component);

  template <typename T, typename = enable_if<is_base_of<Component, T>::value>>
  weak_ptr<T> GetComponent()
  {
    for (auto component : this->components)
    {
      shared_ptr<T> requeredComponent = dynamic_pointer_cast<T>(component);

      if (requeredComponent)
        return requeredComponent;
    }
    weak_ptr<T> nullweak;
    return nullweak;
  }

  std::list<weak_ptr<Component>> GetComponents() const;

  void RemoveComponent(shared_ptr<Component> component);

  const std::string GetName() const;

  void SetName(const std::string newName);

  weak_ptr<Scene> GetScene() const;
  //Called when gameobject added to scene
  void SetScene(shared_ptr<Scene> scene);

  void DrawEditor();

  //Coordinates on scene OR coordinates on scene relative to parent (parent is 0 0 point)
  Transform transform = Transform(sf::Vector2f(0, 0));
  int id = 0;

  std::string name;

protected:
  weak_ptr<Scene> scene;

  weak_ptr<GameObject> parent;
  std::list<shared_ptr<GameObject>> childs;
  std::list<shared_ptr<Component>> components;
  
};
} // namespace eng