#include "Utils.h"
#include <Box2D/Box2D.h>
#include <experimental/filesystem>

#pragma once

namespace eng
{

namespace fs = std::experimental::filesystem;

class Editor
{
  public:
    virtual void DrawEditor(){};

    template <typename V>
    void DrawVector2(sf::Vector2<V> &vector)
    {
        ImGui::InputFloat("X ", &vector.x);
        ImGui::InputFloat("Y ", &vector.y);
    };

    void DrawVector2(const b2Vec2 vector)
    {
        ImGui::LabelText("X ", to_string(vector.x).c_str());
        ImGui::LabelText("Y ", to_string(vector.y).c_str());
    };

    void DrawSfShape(shared_ptr<sf::Shape> &shape)
    {
        static shared_ptr<sf::CircleShape> savedCircle = make_shared<sf::CircleShape>();
        static shared_ptr<sf::RectangleShape> savedRectangle = make_shared<sf::RectangleShape>();

        int selectedType = 0;
        shared_ptr<sf::CircleShape> circle;
        shared_ptr<sf::RectangleShape> rectangle;
        if (circle = dynamic_pointer_cast<sf::CircleShape>(shape))
        {
            selectedType = 0;
        }
        else if (rectangle = dynamic_pointer_cast<sf::RectangleShape>(shape))
        {
            selectedType = 1;
        }
        //Shape
        ImGui::RadioButton("Circle", &selectedType, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Rectangle", &selectedType, 1);
        switch (selectedType)
        {
        case 0:
        {

            if (!circle)
            {
                circle = make_shared<sf::CircleShape>();
                circle->setRadius(savedCircle->getRadius());
                circle->setOrigin(savedCircle->getOrigin());
                circle->setFillColor(savedCircle->getFillColor());
                circle->setOutlineColor(savedCircle->getOutlineColor());
                circle->setOutlineThickness(savedCircle->getOutlineThickness());
                shape = circle;
            }
            float radius = circle->getRadius();
            if (ImGui::InputFloat("Radius", &radius))
            {
                circle->setRadius(radius);
                circle->setOrigin(radius, radius);
            }
            savedCircle = make_shared<sf::CircleShape>();

            savedCircle->setRadius(circle->getRadius());
            savedCircle->setOrigin(circle->getOrigin());
            savedCircle->setFillColor(circle->getFillColor());
            savedCircle->setOutlineColor(circle->getOutlineColor());
            savedCircle->setOutlineThickness(circle->getOutlineThickness());
            break;
        }
        case 1:
        {
            if (!rectangle)
            {
                rectangle = make_shared<sf::RectangleShape>();
                rectangle->setSize(savedRectangle->getSize());
                rectangle->setOrigin(savedRectangle->getOrigin());
                rectangle->setFillColor(savedRectangle->getFillColor());
                rectangle->setOutlineColor(savedRectangle->getOutlineColor());
                rectangle->setOutlineThickness(savedRectangle->getOutlineThickness());
                shape = rectangle;
            }
            sf::Vector2f size = rectangle->getSize();

            if (ImGui::InputFloat("Width ", &size.x) || ImGui::InputFloat("Height ", &size.y))
            {
                rectangle->setSize(size);
                rectangle->setOrigin(size.x / 2, size.y / 2);
            }

            savedRectangle->setSize(rectangle->getSize());
            savedRectangle->setOrigin(rectangle->getOrigin());
            savedRectangle->setFillColor(rectangle->getFillColor());
            savedRectangle->setOutlineColor(rectangle->getOutlineColor());
            savedRectangle->setOutlineThickness(rectangle->getOutlineThickness());
            break;
        }
        }
        //Color
        sf::Color sfFillColor = shape->getFillColor();
        float colorFill[3] = {sfFillColor.r / 255.f, sfFillColor.g / 255.f, sfFillColor.b / 255.f};
        if (ImGui::ColorEdit3("FillColor", colorFill))
        {
            sfFillColor.r = static_cast<sf::Uint8>(colorFill[0] * 255.f);
            sfFillColor.b = static_cast<sf::Uint8>(colorFill[1] * 255.f);
            sfFillColor.g = static_cast<sf::Uint8>(colorFill[2] * 255.f);
            shape->setFillColor(sfFillColor);
        }
        sf::Color sfOutlineColor = shape->getOutlineColor();
        float outLineThickness = shape->getOutlineThickness();
        if (ImGui::InputFloat("OutLineThickness", &outLineThickness))
        {
            shape->setOutlineThickness(outLineThickness);
        }
        float colorOutline[3] = {sfOutlineColor.r / 255.f, sfOutlineColor.g / 255.f, sfOutlineColor.b / 255.f};
        if (ImGui::ColorEdit3("OutlineColor", colorOutline))
        {
            sfOutlineColor.r = static_cast<sf::Uint8>(colorOutline[0] * 255.f);
            sfOutlineColor.b = static_cast<sf::Uint8>(colorOutline[1] * 255.f);
            sfOutlineColor.g = static_cast<sf::Uint8>(colorOutline[2] * 255.f);
            shape->setOutlineColor(sfOutlineColor);
        }
    }

    static void DrawInspector();
    static bool DrawOpenFileDialog(fs::path path, string& file);
};
} // namespace eng