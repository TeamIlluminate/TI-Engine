#include "Utils.h"
#include "GameObject.h"
#pragma once
namespace eng{
    class Component
    {
        public:

        void SetParent(GameObject * attachto);
        //Called before frame render
        virtual void Update();
        //Called after physics calculating
        virtual void FixedUpdate();
        //Called when Component attached to gameobject
        virtual void OnInit();
        //
        virtual void BeginContact(GameObject * gameObject);
        //
        virtual void EndContact(GameObject * gameObject);

        protected:
        GameObject * attached = nullptr;
        
    };
}