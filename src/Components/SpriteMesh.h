#include "Mesh.h"

namespace eng
{

class SpriteMesh : public Mesh
{
  public:
    sf::Drawable *GetDrawable();
    sf::RenderStates GetRenderStates();
};
} // namespace eng