#include "../Component.h"
#include <SFML/Graphics.hpp>
namespace eng
{
//[Serializable]
class Camera : public Component, public enable_shared_from_this<Camera>
{
  public:
    shared_ptr<Component> Clone();
    void OnInit();

    int maxScale = 120;
    int minScale = 30;
    int scale = 120;
    sf::View view;
    bool isEnabled = false;
    json Serialize();
    void Deserialize(json obj);

    void Update();
    void DrawEditor();
};
} // namespace eng