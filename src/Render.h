#include "Utils.h"
#include <thread>

namespace eng
{

class Scene;
class GameObject;

class Render
{
public:
  Render(sf::VideoMode mode);

  void SetScene(Scene *scene);

private:
  sf::RenderWindow *window = nullptr;

  std::thread *wThread;
  Scene *currentScene = nullptr;

  void WindowLoop();
  void Draw(shared_ptr<GameObject> object);
};

} // namespace eng