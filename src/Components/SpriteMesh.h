#include "Utils.h"
#include "Mesh.h"

namespace eng
{
class SpriteMesh : public Mesh, public enable_shared_from_this<SpriteMesh>
{
    public:
    SpriteMesh(){
        sprite = make_shared<sf::Sprite>();
        e_sprite = make_shared<sf::Sprite>();
        };
        
    SpriteMesh(string file){ sprite = make_shared<sf::Sprite>();};
    weak_ptr<sf::Drawable> GetDrawable();
    sf::RenderStates GetRenderStates();
    shared_ptr<Component> Clone();

    private:
    void OnInit();
    void DrawEditor();
    shared_ptr<sf::Sprite> sprite;
    shared_ptr<sf::Sprite> e_sprite;
    bool open = false;
    sf::RenderStates renderStates;
    shared_ptr<sf::Sprite> loadSprite(string file);
    sf::Texture texture;
};
} // namespace eng