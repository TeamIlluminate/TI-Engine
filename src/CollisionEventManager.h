#include <Box2D/Box2D.h>
#pragma once
namespace eng {

class Scene;

    class CollisionEventManager : public b2ContactListener {
        public:

        CollisionEventManager(Scene * scene);
        void BeginContact(b2Contact * contact);
        void EndContact(b2Contact * contact);

        private:
        Scene * scene;
    };
}