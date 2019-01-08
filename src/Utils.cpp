#include "Utils.h"
#include "GameMaster.h"

using namespace eng;

sf::Vector2f GetMouseCoordinates() {
    if (auto window = GameMaster::Get().GetWindow().lock()) {
    return window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    } else return sf::Vector2f(0,0);
}