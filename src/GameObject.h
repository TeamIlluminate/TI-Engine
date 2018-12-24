#include "Utils.h"
#include "Transform.h"
#pragma once
namespace eng
{

class Component;

    class GameObject
    {
        public:

            GameObject();
            GameObject(const std::string name);

            //Add component to components list and set to him this gameobject as parent
            void AddNewComponent(Component * component);

            template <class T>
            T *GetComponent()
            {
                for (Component* component : this->components)
                {
                    T *requeredComponent = dynamic_cast<T *>(*component);
                    if (requeredComponent) return requeredComponent;
                }
                return nullptr;
            }


            //return true only if find and deleted 
            bool RemoveComponent(Component * component);
            const std::string GetName() const;
            void SetName(const std::string newName);
            Transform transform = Transform(sf::Vector2f(0, 0));

        private:

            std::string name;
            std::list<GameObject *> childs;
            std::list<Component *> components;

    };
} // namespace eng