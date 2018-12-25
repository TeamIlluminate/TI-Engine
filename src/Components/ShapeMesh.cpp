#include "ShapeMesh.h"
#include "GameObject.h"

using namespace eng;

ShapeMesh::ShapeMesh(sf::Shape* shape) : currentShape(shape) {}

sf::Drawable* ShapeMesh::GetDrawable() {
    currentShape->setPosition(attached->GetGlobalCoordinates());
    return currentShape; 
}
sf::RenderStates ShapeMesh::GetRenderStates() { return sf::RenderStates::Default; }