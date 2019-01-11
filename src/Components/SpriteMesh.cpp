#include "SpriteMesh.h"

using namespace eng;

void SpriteMesh::DrawEditor()
{
    if(ImGui::CollapsingHeader("SpriteMesh"))
    {
        if(sprite) ImGui::Image(*sprite.get(), sf::Color::Transparent);
        if(ImGui::Button("Choose"))
        {
            open = true;
        }

        sf::Texture texture;
        string file;

        if(DrawOpenFileDialog("Resource", open, file, "SpriteMesh_" + to_string(_owner.lock()->id)))
        {
            if(texture.loadFromFile(file))
            {     
                sprite->setColor(sf::Color::Transparent);
                sprite->setTexture(texture);
                sprite->setPosition(500, 500);
                GameMaster::Get().GetWindow().lock()->draw(*sprite.get());
            } 
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