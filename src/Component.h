#include "Utils.h"
#include "GameObject.h"
#pragma once
namespace eng
{
class Component
{
public:
  virtual ~Component(){}
  void SetParent(GameObject *attachTo)
  {
    this->attached = attachTo;
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
  virtual void BeginContact(GameObject *gameObject){};
  //
  virtual void EndContact(GameObject *gameObject){};

protected:
  GameObject *attached = nullptr;
};
} // namespace eng