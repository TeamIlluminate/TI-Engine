#include "Utils.h"
#include "CollisionEventManager.h"
#include <Box2D/Box2D.h>

namespace eng{

class GameObject;
class Camera;

    class Scene
    {
        public:
        
        Scene(const std::string name); //Name 4r Serialization ability

        std::list<GameObject*> GetGameObjects() const; //Generaly 4r Render using or serializing

        void AddGameObject(GameObject* object);

        /* GameObjects GETTERS */

        GameObject* FindGameObject(std::string objectName) const;
        std::list<GameObject*> FindGameObjects(std::string objectName) const;

        b2World* GetWorld() const;

        void PhysicsLoop();

        void RemoveFromScene(GameObject* objectToRemove); //Remove gameobject from current Scene and free memory

        private:

        b2World* world;
        CollisionEventManager* collisionEventManager; 
        std::list<GameObject*> sceneObjects;
        std::string name;
        sf::Thread* physicThread;
        int timestep = 60;int velocityIterations = 6;int positionIterations = 2;
    };

}