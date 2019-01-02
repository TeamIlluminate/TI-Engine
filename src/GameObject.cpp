#include "GameObject.h"
#include "Component.h"

using namespace eng;

sf::Vector2f GameObject::GetGlobalCoordinates() const
{
    if (shared_ptr<GameObject> prn = parent.lock())
    {
        transform.position + prn->GetGlobalCoordinates();
    }
    else
        return transform.position;
}

const std::string GameObject::GetName() const
{
    return name;
}

void GameObject::SetName(std::string newName)
{
    this->name = newName;
}

void GameObject::AddChild(shared_ptr<GameObject> child)
{
    // if ( auto prn = child->parent.lock() ){
    //     prn->RemoveChild(child);
    // }

    childs.push_back(child);

    child->parent = weak_from_this();
}

void GameObject::RemoveChild(GameObject *child)
{
}

std::list<weak_ptr<GameObject>> GameObject::GetChilds() const
{
    list<weak_ptr<GameObject>> weaks;
    for (auto shared : this->childs)
    {
        weaks.push_back((weak_ptr<GameObject>)shared);
    }

    return weaks;
}

weak_ptr<GameObject> GameObject::GetParent() const
{
    return this->parent;
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
    component->SetOwner(weak_from_this());
    this->components.push_back(component);
}

std::list<weak_ptr<Component>> GameObject::GetComponents() const
{
    list<weak_ptr<Component>> weaks;

    for (auto shared : this->components)
    {
        weaks.push_back((weak_ptr<Component>)shared);
    }

    return weaks;
}

void GameObject::RemoveComponent(Component *component)
{
    // this->components.remove(component);
    // component->~Component();
}

weak_ptr<Scene> GameObject::GetScene() const
{
    return scene;
}

void GameObject::SetScene(shared_ptr<Scene> scene)
{
    this->scene = scene;
}

void GameObject::DrawEditor()
{
    if (ImGui::TreeNode(to_string(id).c_str())) {
        ImGui::Text(name.c_str());
        DrawVector2(transform.position);
        if (ImGui::TreeNode(("Components " + to_string(id)).c_str()))
        {
            for (auto component : components)
            {
                component->DrawEditor();
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
}