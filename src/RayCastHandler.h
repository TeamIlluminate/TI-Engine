#include <Box2D/Box2D.h>
#include "GameObject.h"
namespace eng {
class RayCastHandler : public b2RayCastCallback {
    public:
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction); 
    ~RayCastHandler(){};
    GameObject * foundedObject;
};
}   