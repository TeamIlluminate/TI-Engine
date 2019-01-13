#include "Components/Mesh.h"

using namespace eng;

Mesh::~Mesh()
{
    if (auto scene = GameMaster::Get().GetCurrentScene().lock())
    {
        //scene->DeleteB2B(body);
    }
}

void Mesh::OnInit()
{
    /*
    b2Shape * shape;    
    if (type == 0) {
        shape = new b2CircleShape();
        shape->m_radius = dynamic_pointer_cast<sf::CircleShape>(shape)->getRadius();                                    
    }

        b2BodyDef defBody;
        defBody.type = b2_dynamicBody;
        defBody.position.Set(_owner.lock()->transform.position.x, _owner.lock()->transform.position.y);
        this->body = _owner.lock()->GetScene().lock()->GetWorld().lock()->CreateBody(&defBody);
        b2FixtureDef defFixture;*/

    // b2BodyDef defBody;
    // defBody.type = this->type;
    // defBody.position.Set(_owner.lock()->transform.position.x, _owner.lock()->transform.position.y);
    // this->body = _owner.lock()->GetScene().lock()->GetWorld().lock()->CreateBody(&defBody);
    // fixture = this->body->CreateFixture(&fixtureDef);
    // this->body->SetGravityScale(1);
    // this->body->SetLinearDamping(0.1f);
}

shared_ptr<Component> Mesh::Clone()
{
    return make_shared<Mesh>(*this);
}

shared_ptr<sf::Sprite> Mesh::LoadSprite(string file)
{
}

void Mesh::DrawEditor()
{
    if (ImGui::CollapsingHeader("Mesh"))
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
    }

    sf::Vector2f size = editable->getSize();

    if (ImGui::InputFloat("Width ", &size.x) || ImGui::InputFloat("Height ", &size.y))
    {
        editable->setSize(size);
        editable->setOrigin(size.x / 2, size.y / 2);
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
    }
}

void Mesh::EditorSprite()
{
    auto editable = make_shared<sf::RectangleShape>();

    if(!shape->getTexture())
    {
        editable->setSize(sf::Vector2f(128.f, 128.f));
        editable->setOrigin(128.f / 2, 128.f / 2);
        editable->setFillColor(sf::Color(255.f, 255.f, 255.f, 255.f));
    } else editable = dynamic_pointer_cast<sf::RectangleShape>(shape);

    sf::Vector2f size = editable->getSize();

    if (ImGui::InputFloat("Width ", &size.x) || ImGui::InputFloat("Height ", &size.y))
    {
        editable->setSize(size);
        editable->setOrigin(size.x / 2, size.y / 2);
    }

    if (editable->getTexture()) {
        ImGui::Image(*editable->getTexture(), sf::Vector2f(128.f, 128.f));
    }

    if (ImGui::Button("Choose"))
    {       
        OpenFD();
    }

    string file;
    if ((file = DrawOpenFileDialog("Resource", this)) != "")
    {
        texture->loadFromFile(file);        
    }

    ImGui::SameLine();
    if (ImGui::Button("Apply"))
    {
        editable->setTexture(texture.get(), true);
        shape = editable;
    }
}