#include "Utils.h"
#include "GameObject.h"
#include "Editor.hpp"
#include "GameMaster.h"
#include "Serializable.h"

#pragma once

namespace eng
{
class Component : public Editor, public Serializable
{
  public:

  friend class Scene;
  friend class CollisionEventManager;
  friend class GameObject;
  //Use constructor only for initialize variables
  //Remember field owner is null by default
  //If you want code with using owner variable - put this code in OnInit()
  virtual ~Component(){};

  void SetOwner(weak_ptr<GameObject> newOwner)
  {
    this->_owner = newOwner;
  }

  virtual float DeltaTime()
  {
    return GameMaster::Get().GetDeltaTime();
  };

  weak_ptr<GameObject> _owner;

  private:

  //Called when Component appears on scene with gameobject
  virtual void OnInit(){};
  //Called before frame render
  virtual void Update(){};
  //Called after physics calculating
  virtual void FixedUpdate(){};
  //Called on GUI
  virtual void GUI(){};
  //
  virtual void BeginContact(weak_ptr<GameObject>  gameObject){};
  //
  virtual void EndContact(weak_ptr<GameObject> gameObject){};

  virtual shared_ptr<Component> Clone() = 0;
};
} // namespace eng