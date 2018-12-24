#include "GameObject.h"
#include "Component.h"

using namespace eng;

GameObject::GameObject() : name("Empty"){}
GameObject::GameObject(std::string newName) : name(newName){}

void GameObject::AddNewComponent(Component *component) {
    component->SetParent(this);
    this->components.push_back(component);
}

const std::string GameObject::GetName() const{
    return name;
}

void GameObject::SetName(std::string newName) {
    this->name = newName;
}