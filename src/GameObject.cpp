#include "GameObject.h"
#include "Component.h"

using namespace eng;

GameObject::GameObject() : name("Empty"){}
GameObject::GameObject(std::string newName) : name(newName){}

void GameObject::AddNewComponent(Component *component) {
    component->SetParent(this);
    this->components.push_back(component);
    component->OnInit();
}

sf::Vector2f GameObject::GetGlobalCoordinates() {
    return parent ? transform.position + parent->GetGlobalCoordinates() : transform.position;
}

const std::string GameObject::GetName() const {
    return name;
}

void GameObject::SetName(std::string newName) {
    this->name = newName;
}

void GameObject::AddChild(GameObject * child) {
    childs.push_back(child);
    child->parent = this;
}

GameObject * GameObject::GetParent() {

    return this->parent;   
}

std::list<GameObject*> GameObject::GetChilds() {
    return this->childs;
}

std::list<Component* > GameObject::GetComponents() const
{
    return this->components;
}