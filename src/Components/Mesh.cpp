#include "Components/Mesh.h"
#include "RayCastHandler.h"
#include "ResourceManager.h"

using namespace eng;

Mesh::~Mesh()
{
    if (auto scene = GameMaster::Get().GetCurrentScene().lock())
    {
        scene->DeleteB2B(body);
    }
}

void Mesh::OnInit()
{
    if (!shape)
    {
        shape = make_shared<sf::CircleShape>();
        auto initShape = dynamic_pointer_cast<sf::CircleShape>(this->shape);
        if (initShape)
        {
            initShape->setRadius(10.f);
            initShape->setOrigin(10.f, 10.f);
        }
    }

    if (physEnable)
        if (this->body)
        {
            CreatePhysics();
        }
}

void Mesh::Configure(shared_ptr<sf::Shape> inShape)
{
    this->shape = inShape;
    if (auto circle = dynamic_pointer_cast<sf::CircleShape>(inShape))
    {
        circle->setOrigin(circle->getRadius(), circle->getRadius());
    }
    else if (auto rectangle = dynamic_pointer_cast<sf::RectangleShape>(this->shape))
    {
        auto size = rectangle->getSize();
        rectangle->setOrigin(size.x / 2.f, size.y / 2.f);
    }

    if (physEnable)
        CreatePhysics();
}

void Mesh::CreatePhysics()
{
    if (this->body)
    {
        if (this->body->GetWorld() == GameMaster::Get().GetCurrentScene().lock()->GetWorld().lock().get())
        {
            this->body->GetWorld()->DestroyBody(this->body);
        }
    }

    b2Shape *shape;
    if (auto circle = dynamic_pointer_cast<sf::CircleShape>(this->shape))
    {
        shape = new b2CircleShape();
        shape->m_radius = circle->getRadius() / physicsCoef;
    }
    else if (auto rectangle = dynamic_pointer_cast<sf::RectangleShape>(this->shape))
    {
        b2PolygonShape *rect = new b2PolygonShape();
        auto size = rectangle->getSize();
        rect->SetAsBox(size.x / (2.f * physicsCoef), size.y / (2.f * physicsCoef));
        shape = rect;
    }

    b2BodyDef defBody;
    defBody.type = b2_dynamicBody;
    defBody.position.Set(_owner.lock()->transform.position.x / physicsCoef, _owner.lock()->transform.position.y / physicsCoef);
    defBody.angle = _owner.lock()->transform.angle / b2_pi;
    this->body = _owner.lock()->GetScene().lock()->GetWorld().lock()->CreateBody(&defBody);
    b2FixtureDef defFixture;
    defFixture.shape = shape;
    this->fixture = this->body->CreateFixture(&defFixture);
}

shared_ptr<Component> Mesh::Clone()
{
    auto clone = make_shared<Mesh>(*this);
    if (auto circle = dynamic_pointer_cast<sf::CircleShape>(this->shape))
    {
        clone->shape = make_shared<sf::CircleShape>(*circle.get());
    }
    else if (auto rectangle = dynamic_pointer_cast<sf::RectangleShape>(this->shape))
    {
        clone->shape = make_shared<sf::RectangleShape>(*rectangle.get());
        ;
    }

    if(textureKey != "")
    {
        clone->textureKey = textureKey;
        clone->shape->setTexture(ResourceManager::Get().GetTexture(textureKey).lock()->sf_texture.get(), true);
    }
    return clone;
}

shared_ptr<sf::Sprite> Mesh::LoadSprite(string file)
{
}

weak_ptr<GameObject> Mesh::RayCast(sf::Vector2f to)
{
    RayCastHandler *handler = new RayCastHandler();
    this->body->GetWorld()->RayCast(handler, this->body->GetPosition(), b2Vec2(to.x, to.y));
    auto gameObject = handler->foundedObject;
    delete handler;
    return gameObject;
}

void Mesh::TransformPosition(sf::Vector2f newPos)
{
    if (!this->body->GetWorld()->IsLocked())
    {
        if (auto owner = _owner.lock())
        {
            newPos =  newPos / physicsCoef;
            b2Vec2 position = body->GetPosition() + b2Vec2(newPos.x, newPos.y);
            this->body->SetTransform(position, body->GetAngle());
        }
    }
}

void Mesh::FixedUpdate()
{
    if (this->body)
    {
        b2Vec2 position = body->GetPosition();
        _owner.lock()->transform.position = sf::Vector2f(position.x * physicsCoef, position.y * physicsCoef);
        _owner.lock()->transform.angle = body->GetAngle() * b2_pi;
    }
}

void Mesh::AddImpulse(sf::Vector2f impulse)
{
    this->body->ApplyLinearImpulse(b2Vec2(impulse.x / physicsCoef, impulse.y / physicsCoef), this->body->GetWorldCenter(), true);
}

void Mesh::DrawEditor()
{
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_Bullet))
    {
        const char *items[] = {"Circle", "Rectangle", "Sprite"};

        ImGui::Combo("MeshType", &type, items, IM_ARRAYSIZE(items));

        switch (static_cast<MeshType>(type))
        {
        case Circle:
        {
            EditorCircle();
            break;
        }
        case Rectangle:
        {
            EditorRect();
            break;
        }
        case Sprite:
        {
            EditorSprite();
            break;
        }
        }
        ImGui::Checkbox("Enable physics?", &physEnable);
        if (physEnable)
            EditorPhysics();
    }
}

void Mesh::EditorCircle()
{

    auto editable = make_shared<sf::CircleShape>();

    if (auto circle = dynamic_pointer_cast<sf::CircleShape>(shape))
    {
        editable = circle;
    }
    else
    {
        editable->setRadius(10.f);
        editable->setOrigin(10.f, 10.f);
        editable->setFillColor(sf::Color(255.f, 255.f, 255.f));
    }

    float radius = editable->getRadius();
    if (ImGui::InputFloat("Radius", &radius))
    {
        editable->setRadius(radius);
        editable->setOrigin(radius, radius);
    }

    sf::Color sfFillColor = editable->getFillColor();
    float colorFill[3] = {sfFillColor.r / 255.f, sfFillColor.g / 255.f, sfFillColor.b / 255.f};
    if (ImGui::ColorEdit3("FillColor", colorFill))
    {
        sfFillColor.r = static_cast<sf::Uint8>(colorFill[0] * 255.f);
        sfFillColor.b = static_cast<sf::Uint8>(colorFill[1] * 255.f);
        sfFillColor.g = static_cast<sf::Uint8>(colorFill[2] * 255.f);
        editable->setFillColor(sfFillColor);
    }
    sf::Color sfOutlineColor = editable->getOutlineColor();
    float outLineThickness = editable->getOutlineThickness();
    if (ImGui::InputFloat("OutLineThickness", &outLineThickness))
    {
        editable->setOutlineThickness(outLineThickness);
    }
    float colorOutline[3] = {sfOutlineColor.r / 255.f, sfOutlineColor.g / 255.f, sfOutlineColor.b / 255.f};
    if (ImGui::ColorEdit3("OutlineColor", colorOutline))
    {
        sfOutlineColor.r = static_cast<sf::Uint8>(colorOutline[0] * 255.f);
        sfOutlineColor.b = static_cast<sf::Uint8>(colorOutline[1] * 255.f);
        sfOutlineColor.g = static_cast<sf::Uint8>(colorOutline[2] * 255.f);
        editable->setOutlineColor(sfOutlineColor);
    }

    if (ImGui::Button("Apply"))
    {
        shape = editable;
        if (physEnable)
            CreatePhysics();
    }
}

void Mesh::EditorRect()
{
    auto editable = make_shared<sf::RectangleShape>();

    if (auto rectangle = dynamic_pointer_cast<sf::RectangleShape>(shape))
    {
        editable = rectangle;
    }
    else
    {
        editable->setSize(sf::Vector2f(10.f, 10.f));
        editable->setFillColor(sf::Color(255.f, 255.f, 255.f));
        editable->setOrigin(5.f, 5.f);
    }

    sf::Vector2f size = editable->getSize();

    if (ImGui::InputFloat("Width ", &size.x) || ImGui::InputFloat("Height ", &size.y))
    {
        editable->setSize(size);
        editable->setOrigin(size.x, size.y);
    }

    sf::Color sfFillColor = editable->getFillColor();
    float colorFill[3] = {sfFillColor.r / 255.f, sfFillColor.g / 255.f, sfFillColor.b / 255.f};
    if (ImGui::ColorEdit3("FillColor", colorFill))
    {
        sfFillColor.r = static_cast<sf::Uint8>(colorFill[0] * 255.f);
        sfFillColor.b = static_cast<sf::Uint8>(colorFill[1] * 255.f);
        sfFillColor.g = static_cast<sf::Uint8>(colorFill[2] * 255.f);
        editable->setFillColor(sfFillColor);
    }
    sf::Color sfOutlineColor = editable->getOutlineColor();
    float outLineThickness = editable->getOutlineThickness();
    if (ImGui::InputFloat("OutLineThickness", &outLineThickness))
    {
        editable->setOutlineThickness(outLineThickness);
    }
    float colorOutline[3] = {sfOutlineColor.r / 255.f, sfOutlineColor.g / 255.f, sfOutlineColor.b / 255.f};
    if (ImGui::ColorEdit3("OutlineColor", colorOutline))
    {
        sfOutlineColor.r = static_cast<sf::Uint8>(colorOutline[0] * 255.f);
        sfOutlineColor.b = static_cast<sf::Uint8>(colorOutline[1] * 255.f);
        sfOutlineColor.g = static_cast<sf::Uint8>(colorOutline[2] * 255.f);
        editable->setOutlineColor(sfOutlineColor);
    }

    if (ImGui::Button("Apply"))
    {
        shape = editable;
        if (physEnable)
            CreatePhysics();
    }
}

void Mesh::EditorSprite()
{
    auto editable = make_shared<sf::RectangleShape>();

    if (!shape->getTexture())
    {
        editable->setSize(sf::Vector2f(128.f, 128.f));
        editable->setOrigin(128.f / 2, 128.f / 2);
        editable->setFillColor(sf::Color(255.f, 255.f, 255.f, 255.f));
    }
    else
        editable = dynamic_pointer_cast<sf::RectangleShape>(shape);

    sf::Vector2f size = editable->getSize();

    if (ImGui::InputFloat("Width ", &size.x) || ImGui::InputFloat("Height ", &size.y))
    {
        editable->setSize(size);
        editable->setOrigin(size.x / 2, size.y / 2);
    }

    if (editable->getTexture())
    {
        ImGui::Image(*editable->getTexture(), sf::Vector2f(128.f, 128.f));
        ImGui::SameLine();
    }

    if (ImGui::Button("Choose"))
    {
        OpenFileDialog("Resource", this);
    }

    if (OFD_Status())
    {
        if(auto txtr = ResourceManager::Get().LoadTexture(*GetOFD_Result()).lock())
        {
            auto texture = txtr->sf_texture.get();
            editable->setTexture(texture, true);
            textureKey = txtr->key;
            shape = editable;
            if (physEnable)
                CreatePhysics();
        }
    }
    
}

void Mesh::EditorPhysics()
{
    ImGui::Separator();
    if (!this->body)
        CreatePhysics();

    ImGui::Text("Velocity");
    b2Vec2 velocity = this->body->GetLinearVelocity();

    if (ImGui::InputFloat("X ##velocity", &velocity.x) || (ImGui::InputFloat("Y ##velocity", &velocity.y)))
    {
        this->body->SetLinearVelocity(velocity);
    }

    float linearDamping = this->body->GetLinearDamping();
    if (ImGui::InputFloat("Linear Damping ", &linearDamping))
    {
        this->body->SetLinearDamping(linearDamping);
    }

    float mass = this->body->GetMass();
    if (ImGui::InputFloat("Mass ", &mass))
    {
    }

    ImGui::Text("Position");
    b2Vec2 position = this->body->GetPosition();
    if (ImGui::InputFloat("X ##position", &position.x) || (ImGui::InputFloat("Y ##position", &position.y)))
    {
        this->body->SetTransform(position, 0);
    }

    float gravity = this->body->GetGravityScale();
    if (ImGui::InputFloat("Gravity ", &gravity))
    {
        this->body->SetGravityScale(gravity);
    }

    ImGui::Text("Collider");
    float density = this->fixture->GetDensity();
    if (ImGui::InputFloat("Density ", &density))
    {
        this->fixture->SetDensity(density);
    }

    float friction = this->fixture->GetFriction();
    if (ImGui::InputFloat("Friction ", &friction))
    {
        this->fixture->SetFriction(friction);
    }

    auto shape = this->fixture->GetShape();
    b2Shape::Type type = shape->GetType();
    switch (type)
    {
    case b2Shape::Type::e_circle:
    {
        auto circleShape = dynamic_cast<b2CircleShape *>(shape);
        if (auto window = GameMaster::Get().GetWindow().lock())
        {
            ImGui::InputFloat("Collider Radius", &circleShape->m_radius);

            sf::CircleShape collider(physicsCoef * circleShape->m_radius);
            collider.setOrigin(physicsCoef * circleShape->m_radius, physicsCoef * circleShape->m_radius);
            collider.setFillColor(sf::Color::Transparent);
            collider.setOutlineColor(sf::Color(0.f, 255.f, 0.f, 125.f));
            collider.setOutlineThickness(1.f);
            collider.setPosition(_owner.lock()->transform.position);
            collider.setRotation(_owner.lock()->transform.angle);
            window->draw(collider);
        }
        break;
    }
    case b2Shape::Type::e_polygon:
    {
        auto polygonShape = dynamic_cast<b2PolygonShape *>(shape);
        if (auto window = GameMaster::Get().GetWindow().lock())
        {
            auto size = polygonShape->GetVertex(2);
            size.x = physicsCoef * size.x * 2.f;
            size.y = physicsCoef * size.y * 2.f;
            ImGui::PushID("PHYSICS");
            if (ImGui::InputFloat("Width ", &size.x) || ImGui::InputFloat("Height ", &size.y))
            {
                polygonShape->SetAsBox( size.x / (2.f * physicsCoef) , size.y / (2.f * physicsCoef));
            }
            ImGui::PopID();
            sf::RectangleShape collider;
            collider.setSize(sf::Vector2f(size.x, size.y));
            collider.setOrigin(size.x / 2.f, size.y / 2.f);
            collider.setFillColor(sf::Color::Transparent);
            collider.setOutlineColor(sf::Color(0.f, 255.f, 0.f, 125.f));
            collider.setOutlineThickness(1.f);
            collider.setPosition(_owner.lock()->transform.position);
            collider.setRotation(_owner.lock()->transform.angle);
            window->draw(collider);
        }
        break;
    }
    }
}

json Mesh::Serialize()
{
    json meshok;
    meshok["type"] = "Mesh";
    meshok["textureKey"] = textureKey;
    meshok["physEnable"] = physEnable;
    if (auto circle = dynamic_pointer_cast<sf::CircleShape>(shape))
    {
        meshok["shape"]["type"] = "Circle";
        meshok["shape"]["radius"] = circle->getRadius();
    }
    else if (auto rectangle = dynamic_pointer_cast<sf::RectangleShape>(shape))
    {
        meshok["shape"]["type"] = "Rectangle";
        auto size = rectangle->getSize();
        meshok["shape"]["width"] = size.x;
        meshok["shape"]["height"] = size.y;
    }
    auto origin = shape->getOrigin();
    meshok["shape"]["origin"]["x"] = origin.x;
    meshok["shape"]["origin"]["y"] = origin.y;

    meshok["shape"]["color"]["r"] = shape->getFillColor().r;
    meshok["shape"]["color"]["g"] = shape->getFillColor().g;
    meshok["shape"]["color"]["b"] = shape->getFillColor().b;
    meshok["shape"]["color"]["a"] = shape->getFillColor().a;

    if (physEnable)
    {

        meshok["body"]["transform"]["x"] = body->GetPosition().x;
        meshok["body"]["transform"]["y"] = body->GetPosition().y;
        meshok["body"]["angle"] = body->GetTransform().q.GetAngle();

        meshok["body"]["velocity"]["x"] = body->GetLinearVelocity().x;
        meshok["body"]["velocity"]["y"] = body->GetLinearVelocity().y;
        
        auto collider = fixture->GetShape();
        switch (collider->m_type)
        {
        case b2Shape::Type::e_circle:
        {
            meshok["fixture"]["shape"]["type"] = "e_circle";
            meshok["fixture"]["shape"]["m_radius"] = collider->m_radius;
            break;
        }

        case b2Shape::Type::e_polygon:
        {
            auto polygonShape = dynamic_cast<b2PolygonShape *>(collider);
            meshok["fixture"]["shape"]["type"] = "e_polygon";
            auto size = polygonShape->GetVertex(2);
            meshok["fixture"]["shape"]["width"] = size.x;
            meshok["fixture"]["shape"]["height"] = size.y;
            break;
        }
        }
    }
    return meshok;
}

void Mesh::Deserialize(json s)
{
    std::string shapeType = s["shape"]["type"];
    
    physEnable = s["physEnable"];
    if (shapeType == "Circle")
    {
        auto circle = make_shared<sf::CircleShape>(s["shape"]["radius"]);
        shape = circle;
    }
    else if (shapeType == "Rectangle")
    {
        auto rectangle = make_shared<sf::RectangleShape>();
        rectangle->setSize(sf::Vector2f(s["shape"]["width"], s["shape"]["height"]));
        shape = rectangle;
    }

    shape->setOrigin(s["shape"]["origin"]["x"], s["shape"]["origin"]["y"]);
    shape->setFillColor(sf::Color(s["shape"]["color"]["r"], s["shape"]["color"]["g"], s["shape"]["color"]["b"], s["shape"]["color"]["a"]));

    textureKey = s["textureKey"];
    if (textureKey != "") {
        this->shape->setTexture(ResourceManager::Get().GetTexture(textureKey).lock()->sf_texture.get(), true);
    }

    if (s["physEnable"])
    {
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position = b2Vec2(s["body"]["transform"]["x"], s["body"]["transform"]["y"]);
        def.angle = s["body"]["angle"];

        b2FixtureDef fixtureDef;
        auto jsonShape = s["fixture"]["shape"];
        if (jsonShape["type"] == "e_circle")
        {
            b2CircleShape * circleShape = new b2CircleShape();
            circleShape->m_radius = jsonShape["m_radius"];
            fixtureDef.shape = circleShape;
        }
        else if (jsonShape["type"] == "e_polygon")
        {
            b2PolygonShape * rectShape = new b2PolygonShape();
            rectShape->SetAsBox(jsonShape["width"], jsonShape["height"]);
            fixtureDef.shape = rectShape;
        }

       auto world = _owner.lock()->GetScene().lock()->GetWorld().lock();
       this->body = world->CreateBody(&def);
       this->fixture = this->body->CreateFixture(&fixtureDef);
    }
}