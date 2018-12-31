#include "Utils.h"
#include "GameObject.h"
#include "GameMaster.h"

#pragma once

namespace eng
{
class Component
{
public:
  //Use constructor only for initialize variables
  //Remember field owner is null by default
  //If you want code with using owner variable - put this code in OnInit()
  virtual ~Component(){};

  void SetOwner(weak_ptr<GameObject> newOwner)
  {
    this->owner = newOwner;
  }
  //Called when Component attached to gameobject
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

protected:
  weak_ptr<GameObject> owner;
};
} // namespace eng