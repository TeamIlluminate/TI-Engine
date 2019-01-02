#include "Utils.h"
#include "GameObject.h"
#include "Editor.hpp"
#include "GameMaster.h"

#pragma once

namespace eng
{
class Component : public Editor
{
public:
  //Use constructor only for initialize variables
  //Remember field owner is null by default
  //If you want code with using owner variable - put this code in OnInit()
  virtual ~Component(){};

  void SetOwner(weak_ptr<GameObject> newOwner)
  {
    this->_owner = newOwner;
  }
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

  virtual float DeltaTime()
  {
    return GameMaster::Get().GetDeltaTime();
  };

  weak_ptr<GameObject> _owner;
};
} // namespace eng