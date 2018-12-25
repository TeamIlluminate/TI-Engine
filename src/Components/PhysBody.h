#include "Component.h"
#include <Box2D/Box2D.h>

namespace eng{

class GameObject;

    class PhysBody : public Component
    {
        public:

        PhysBody(b2FixtureDef fixture, b2BodyType type);
        b2FixtureDef GetFixture();
        b2Body * body;

        void FixedUpdate();
        void OnInit();
        void BeginContact(GameObject* object);

        void RayCast();

        private:

        b2FixtureDef fixture;
        b2BodyType type;
    };

}