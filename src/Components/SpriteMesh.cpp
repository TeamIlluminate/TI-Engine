#include "SpriteMesh.h"

using namespace eng;

void SpriteMesh::OnInit()
{
   
}

shared_ptr<sf::Sprite> SpriteMesh::loadSprite(string file)
{
    texture.loadFromFile(file);
    auto size = texture.getSize();

    auto sprt = make_shared<sf::Sprite>(texture);
    auto newSprite = make_shared<sf::Sprite>(texture);
    auto newSize = sf::Vector2f(64.f / size.x, 64.f / size.y);

    sprt->setScale(newSize);
    sprt->setOrigin(size.x / 2.f , size.y / 2.f);

    newSprite->setScale(newSize);
    newSprite->setOrigin(size.x / 2.f , size.y / 2.f);
    

    e_sprite = sprt;

    return newSprite;
}

void SpriteMesh::DrawEditor()
{
    if(ImGui::CollapsingHeader("SpriteMesh"))
    {
        if(sprite) ImGui::Image(*e_sprite.get());

        if(ImGui::Button("Choose"))
        {
            OpenFD();
        }

        string file;

        if( (file = DrawOpenFileDialog("Resource", this)) != "")
        {
            sprite = loadSprite(file);
        }
    }
}

weak_ptr<sf::Drawable> SpriteMesh::GetDrawable()
{
    if (auto owner = _owner.lock())
    {
        sprite->setPosition(owner->GetGlobalCoordinates());
    }
    return (weak_ptr<sf::Drawable>)sprite;
}

sf::RenderStates SpriteMesh::GetRenderStates()
{
    return renderStates;
}

shared_ptr<Component> SpriteMesh::Clone()
{
    return make_shared<SpriteMesh>(*this);
}