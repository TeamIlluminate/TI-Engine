#include "GameObject.h"
#include "Components/ShapeMesh.h"
#include "Components/PhysBody.h"

class JustCircle : public eng::GameObject {
    public:
    JustCircle(std::string name, float x, float y, float param, b2BodyType bodyType) : GameObject(name) {

        sf::CircleShape* shape = new sf::CircleShape(param);
        shape->setFillColor(sf::Color::Red);

        this->AddNewComponent( new eng::ShapeMesh(shape));
        this->transform.position = sf::Vector2<float>(x, y);

        b2CircleShape * physShape = new b2CircleShape();
        //physShape->m_p.Set(this->transform.position.x, this->transform.position.y);   
        physShape->m_radius = param;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = physShape;
        fixtureDef.density = 1;
        fixtureDef.friction = 0.3f;

        this->AddNewComponent(new eng::PhysBody(fixtureDef, bodyType));

    }
};


class StaticCircle : public eng::GameObject {
    public:

    StaticCircle(std::string name, float x, float y, float param) : GameObject(name) {

        sf::CircleShape* shape = new sf::CircleShape(param);
        shape->setFillColor(sf::Color::Red);

        this->AddNewComponent( new eng::ShapeMesh(shape));
        this->transform.position = sf::Vector2<float>(x, y);

        b2CircleShape * physShape = new b2CircleShape();
        physShape->m_radius = param;
  
        b2FixtureDef fixtureDef;
        fixtureDef.shape = physShape;
        fixtureDef.density = 1;
        fixtureDef.friction = 0.3f;

        this->AddNewComponent(new eng::PhysBody(fixtureDef, b2_staticBody));

    }
};