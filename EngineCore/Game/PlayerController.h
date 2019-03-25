#include "Utils.h"
#include "Component.h"
#include "Components/Mesh.h"    
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

namespace eng
{
    //[Serializable]
    class PlayerController : public Component
    {
        public:        
        shared_ptr<eng::Component> Clone();
        void OnInit();
        void Update();
        void FixedUpdate();
        void DrawEditor();

        json Serialize();
        void Deserialize(json obj);
        private:
        float speed = 15.f;
        void MoveIn(sf::Vector2f position);
        void ShootIn(sf::Vector2f position);
        string soundBufferKey;

        weak_ptr<Mesh>  _mesh;     
        sf::Sound pew;
        float shootDelay = .1f;
        float bulletForce = 50.f;
        float shoot = 0;
        bool isFiring = false;
        void HandleClick();
        
    };
} // namespace eng;