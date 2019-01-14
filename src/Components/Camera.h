#include "Component.h"
#include <SFML/Graphics.hpp>
namespace eng
{

class Camera : public Component, public enable_shared_from_this<Camera>
{
  public:
    Camera(){};
    shared_ptr<Component> Clone();
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