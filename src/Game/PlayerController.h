#include "Component.h"
#include "Components/PhysBody.h"
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
        private:
        void MoveIn(sf::Vector2f position);
        void ShootIn(sf::Vector2f position);
        weak_ptr<PhysBody>  _bodyComponent;     

        bool isFiring = false;
        const float shootDeley = .1f;
        float shoot = 0;
    
        
    };
} // namespace eng;