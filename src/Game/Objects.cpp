#include "GameObject.h"
#include "Components/ShapeMesh.h"
#include "Components/PhysBody.h"
#include "Components/Camera.h"

class JustCircle : public eng::GameObject {
    public:
    JustCircle(std::string name, float x, float y, float param, b2BodyType bodyType) : GameObject(name) {

        sf::CircleShape* shape = new sf::CircleShape(param);
        shape->setFillColor(sf::Color::Red);    
        shape->setOrigin(param, param);

        this->AddNewComponent( new eng::ShapeMesh(shape));
        this->transform.position = sf::Vector2<float>(x, y);

        b2CircleShape * physShape = new b2CircleShape();
        physShape->m_p.Set(0, 0);   
        physShape->m_radius = param;
        b2FixtureDef fixtureDef;
        fixtureDef.shape = physShape;
        fixtureDef.density = 1000;

        this->AddNewComponent(new eng::PhysBody(fixtureDef, bodyType));

        this->AddNewComponent(new eng::Camera());
    }
};