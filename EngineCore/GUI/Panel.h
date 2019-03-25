#include "Grid.h"
#pragma once

namespace eng
{

class Panel : protected Grid
{
        public:

        Panel(sf::Vector2f _size, string _name, sf::Vector2f _position, ImGuiWindowFlags flags = 0);
        Panel(ImGuiWindowFlags = 0);
        
        void Draw();

        void Hide();
        void Close();
        void Show();

        void SetBorder(bool isBordered);
        bool IsBordered() const;

        ~Panel();

        private:

        bool border = false;
};


}