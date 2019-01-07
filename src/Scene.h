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
  Scene(const std::string name); //Name 4r Serialization ability
 Scene(const Scene & scene);
  std::list<weak_ptr<GameObject>> GetGameObjects() const; //Generaly 4r Render using or serializing

  void AddGameObject(shared_ptr<GameObject> object);

  /* GameObjects GETTERS */

  weak_ptr<GameObject> FindGameObject(std::string objectName) const;
  std::list<weak_ptr<GameObject>> FindGameObjects(std::string objectName) const;

  weak_ptr<b2World> GetWorld() const;

  void AddB2BodyToDelete(b2Body *body);

  void PhysicsLoop();

  list<weak_ptr<Mesh>> Update();

  void Destroy(weak_ptr<GameObject> objectToRemove); //Remove gameobject from current Scene

  list<weak_ptr<Camera>> GetCameras()
  {
    return cameras;
  }

  void ForceGameObject(shared_ptr<GameObject> gameObject);
  void ResetScene();

private:

  list<weak_ptr<Mesh>> Update(shared_ptr<GameObject> object);
  
  void PushGameobjects();
  int idCounter = 0;

  shared_ptr<b2World> world;
  CollisionEventManager *collisionEventManager;

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