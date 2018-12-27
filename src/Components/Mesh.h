#include "Component.h"

namespace eng
{

class Mesh : public Component
{
  public:
  ~Mesh() { std::cout << "!!";}
    virtual sf::Drawable *GetDrawable() = 0;
    virtual sf::RenderStates GetRenderStates() = 0;
};
} // namespace eng