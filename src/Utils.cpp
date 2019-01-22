#include "Utils.h"
#include "GameMaster.h"

using namespace eng;

sf::Vector2f GetMouseCoordinates() {
    if (auto window = GameMaster::Get().GetWindow().lock()) {
    return window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    } else return sf::Vector2f(0,0);
}

sf::Vector2f GetCenterScreenCoordinates() {
    if (auto window = GameMaster::Get().GetWindow().lock()) {
        sf::Vector2u windowSize = window->getSize();
        return window->mapPixelToCoords((sf::Vector2i(windowSize.x / 2.f, windowSize.y / 2.f)));
    }
}