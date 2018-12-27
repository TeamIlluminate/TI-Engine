#include "GameObject.h"
#include "Component.h"

using namespace eng;

GameObject::GameObject(std::string newName) : name(newName) {}
GameObject::~GameObject() {
    if (this->parent) {
        this->parent->RemoveChild(this);
    }
    for (Component * component : components) {
        component->~Component();
    }
    this->scene->RemoveFromScene(this);
    for (GameObject * child : childs) {
        child->~GameObject();
    }
}


sf::Vector2f GameObject::GetGlobalCoordinates() const
{
    return parent ? transform.position + parent->GetGlobalCoordinates() : transform.position;
}

const std::string GameObject::GetName() const
{
    return name;
}

void GameObject::SetName(std::string newName)
{
    this->name = newName;
}

void GameObject::AddChild(GameObject *child)
{
    if (child->parent) {
        child->parent->RemoveChild(child);
    }
    childs.push_back(child);
    child->parent = this;
}

void GameObject::RemoveChild(GameObject * child) {
    this->childs.remove(child);
    child->parent = nullptr;
}

std::list<GameObject *> GameObject::GetChilds() const
{
    return this->childs;
}
GameObject *GameObject::GetParent() const
{

    return this->parent;
}
void GameObject::AddNewComponent(Component *component)
{
    component->SetParent(this);
    this->components.push_back(component);
    component->OnInit();
}

std::list<Component *> GameObject::GetComponents() const
{
    return this->components;
}

void GameObject::RemoveComponent(Component * component) {
    this->components.remove(component);
    component->~Component();
}

Scene * GameObject::GetScene() const {
    return scene;
}

void GameObject::SetScene(Scene * scene){
    if (this->scene) {
        this->scene->RemoveFromScene(this);
    }
    this->scene = scene;
}