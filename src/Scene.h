#include "Utils.h"

namespace eng{

class GameObject;
class Camera;

    class Scene
    {
        public:
        
        Scene(const std::string Name); //Name 4r Serialization ability

        std::list<GameObject*> GetGameObjects() const; //Generaly 4r Render using or serializing

        /* GameObjects GETTERS */

        GameObject* FindGameObject(std::string ObjectName) const;
        std::list<GameObject*> FindGameObjects(std::string ObjectName) const;

        void RemoveFromScene(GameObject* ObjectToRemove); //Remove gameobject from current Scene and free memory

        Camera* GetActiveCamera() const; //Find first Active Camera 4r View

        private:

        std::list<GameObject*> sceneObjects;
        std::string name;

    };

}