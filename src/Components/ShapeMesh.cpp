#include "ShapeMesh.h"
#include "GameObject.h"

using namespace eng;

ShapeMesh::ShapeMesh(shared_ptr<sf::Shape> shape) : currentShape(shape) {
}

weak_ptr<sf::Drawable> ShapeMesh::GetDrawable()
{
    if (auto owner = _owner.lock()) {
        currentShape->setPosition(owner->GetGlobalCoordinates());
    }
    return (weak_ptr<sf::Drawable>) currentShape;
}


shared_ptr<Component> ShapeMesh::Clone() {
    return make_shared<ShapeMesh>(*this);
}


sf::RenderStates ShapeMesh::GetRenderStates() { return states; }

void ShapeMesh::SetShader(shared_ptr<sf::Shader> shader) {
    currentShader = shader;
    states.shader = shader.get();
}