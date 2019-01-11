#include "Utils.h"
#include "Mesh.h"

namespace eng
{
class SpriteMesh : public Mesh
{
    public:
    SpriteMesh(){ sprite = make_shared<sf::Sprite>();};
    weak_ptr<sf::Drawable> GetDrawable();
    sf::RenderStates GetRenderStates();
    shared_ptr<Component> Clone();

    private:
    void DrawEditor();
    shared_ptr<sf::Sprite> sprite;
    bool open = false;
    bool load = false;
    sf::RenderStates renderStates;
};
} // namespace eng