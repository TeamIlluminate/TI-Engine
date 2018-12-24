#include "Utils.h"
#pragma once
namespace eng{
    class GameObject;
    class Component
    {
        public:

        void SetParent(GameObject * parent);

        virtual void Update();
        virtual void FixedUpdate();

        private:

        GameObject * parent = nullptr;
        
    };
}