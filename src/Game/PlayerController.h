#include "Component.h"
#include "Components/Mesh.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
namespace eng
{
    class PlayerController : public Component
    {
        public:
        PlayerController() {}
        shared_ptr<eng::Component> Clone();
        void OnInit();
        void Update();
        void DrawEditor();

        json Serialize();
        void Deserialize(json obj);
        private:
        float speed = 5.f;
        void MoveIn(sf::Vector2f position);
        void ShootIn(sf::Vector2f position);
        weak_ptr<Mesh>  _mesh;     
        float shootDelay = .1f;
        float bulletForce = 50.f;
        float shoot = 0;
        bool isFiring = false;
        
    };
} // namespace eng;