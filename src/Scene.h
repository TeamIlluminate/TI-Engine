#include "Utils.h"
#include "CollisionEventManager.h"
#include <Box2D/Box2D.h>
#include <thread>
#include <mutex>

#pragma once
namespace eng
{

class GameObject;
class Camera;
class Mesh;

class Scene : public enable_shared_from_this<Scene>
{
public:

  friend class Render;
  friend class Editor;
  friend class PhysBody;
  friend class Camera;

  Scene(const std::string name); //Name 4r Serialization ability
  Scene(const Scene & scene);

  std::list<weak_ptr<GameObject>> GetGameObjects() const; //Generaly 4r Render using or serializing


  /* GameObjects GETTERS */

  weak_ptr<GameObject> FindGameObject(std::string objectName) const;
  std::list<weak_ptr<GameObject>> FindGameObjects(std::string objectName) const;

  weak_ptr<b2World> GetWorld() const;

  void Destroy(weak_ptr<GameObject> objectToRemove); //Remove gameobject from current Scene

  shared_ptr<GameObject> CreateGameObject(std::string name);
  shared_ptr<GameObject> CreateGameObject(shared_ptr<GameObject> gameObject);

private:

  int idCounter = 0;
  void Rebind();
  shared_ptr<b2World> world;
  CollisionEventManager *collisionEventManager;

  void DeleteB2B(b2Body *body);
  void RegCamera(shared_ptr<Camera> camera) { this->cameras.push_back(camera); };

  list<weak_ptr<Mesh>> Update();
  void PhysicsLoop();
  list<weak_ptr<Mesh>> Update(shared_ptr<GameObject> object);

  list<weak_ptr<Camera>> GetCameras()
  {
    return cameras;
  }

  int timestep = 60;
  int velocityIterations = 6;
  int positionIterations = 2;

  std::list<shared_ptr<GameObject>> sceneObjects;
  std::list<weak_ptr<Camera>> cameras;
  std::list<b2Body *> toDelete;
  std::list<shared_ptr<GameObject>> neededToAdd;
  std::string name;

  mutex mtx;
};

} // namespace eng