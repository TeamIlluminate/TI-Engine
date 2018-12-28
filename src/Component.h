#include "Utils.h"
#include "GameObject.h"
#include "GameMaster.h"

#pragma once

namespace eng
{
class Component
{
public:

  virtual ~Component(){};

  void SetOwner(weak_ptr<GameObject> newOwner)
  {
    this->owner = newOwner;
  }
  //Called before frame render
  virtual void Update(){};
  //Called after physics calculating
  virtual void FixedUpdate(){};
  //Called when Component attached to gameobject
  virtual void OnInit(){};
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