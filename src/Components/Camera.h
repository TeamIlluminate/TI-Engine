#include "Component.h"
#include "PhysBody.h"
#include <SFML/Graphics.hpp>
namespace eng
{

class Camera : public Component
{
  public:
    void OnInit();
    void Update();
    void DrawEditor();
    int maxScale = 120;
    int minScale = 30;
    int scale = 120;
    sf::View view;
    bool isEnabled = false;
};
} // namespace eng