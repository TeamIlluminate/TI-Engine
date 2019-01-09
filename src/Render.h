#include "Utils.h"
#include <thread>

namespace eng
{

class Scene;
class GameObject;

class Render
{
public:
  void Init(sf::VideoMode mode);

private:
  shared_ptr<sf::RenderWindow> window;
  sf::View defaultView;
  shared_ptr<std::thread> wThread;


  void WindowLoop();

  bool isEditor = false;
};

} // namespace eng