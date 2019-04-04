#include "GameObject.h"
#include "GameMaster.h"
#include "Component.h"
#include "Components/Camera.h"
#include <iterator>

using namespace eng;

GameObject::GameObject()
{
    name = "empty";
    if (auto currentScene = GameMaster::Get().GetCurrentScene().lock())
    {
        this->scene = currentScene;
    }
    else
    {
        throw invalid_argument("Scene has not exists!");
    }
}

GameObject::GameObject(std::string name) : name(name)
{

    if (auto currentScene = GameMaster::Get().GetCurrentScene().lock())
    {
        this->scene = currentScene;
    }
    else
    {
        throw invalid_argument("Scene has not exists!");
    }
}

GameObject::GameObject(weak_ptr<Scene> _scene, string name)
{
    if (auto scene = _scene.lock())
    {
        this->scene = scene;
        this->name = name;
    }
    else
    {
        throw invalid_argument("Scene already deleted!");
    }
}

GameObject::GameObject(const GameObject &gameObject)
{
    this->name = gameObject.name;
    this->scene = gameObject.scene;
    this->id = gameObject.id;
    this->transform.position = gameObject.transform.position;
    this->transform.angle = gameObject.transform.angle;


    for (auto child : gameObject.childs)
    {
        this->childs.push_back(make_shared<GameObject>(*child.get()));
    }

    for (auto component : gameObject.components)
    {
        auto newComponent = component->Clone();
        this->components.push_back(newComponent);
    }
}

sf::Vector2f GameObject::GetGlobalCoordinates() const
{
    if (auto prn = parent.lock())
    {
        return transform.position + prn->GetGlobalCoordinates();
    }
    else
        return transform.position;
}

sf::Vector2f GameObject::GetForwardVector() {
    return Rotate(sf::Vector2f(0.f, -1.f), transform.angle); 
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
    component->OnInit();
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

void GameObject::RemoveComponent(shared_ptr<Component> component)
{
    this->components.remove(component);
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

    ImGui::PushID(id);

    string headerLabel = name + "###" + to_string(id); //только айди у нас не меняется.
    if (ImGui::Button("X"))
    {
        scene.lock()->Destroy(shared_from_this());
    }
    ImGui::SameLine();
    if (ImGui::Button("C"))
    {
        auto newGameObject = GameMaster::Get().GetCurrentScene().lock()->CloneGameObject(shared_from_this());
        newGameObject->transform.position = GetCenterScreenCoordinates();
    }
    ImGui::SameLine();
    bool headerActive = ImGui::CollapsingHeader(headerLabel.c_str());
    if (headerActive)
    {

        ImGui::InputText("NAME:", &name);
        DrawVector2(transform.position);
        ImGui::InputFloat("Angle", &transform.angle);
        ImGui::Separator();
        if (ImGui::TreeNode("Components"))
        {
            for (auto component : components)
            {
                component->DrawEditor();
                component->UpdateEditor();
            }
            ImGui::TreePop();
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader("Component management"))
        {
            static int type = 0;

            ImGui::Combo("Component", &type, GameMaster::Get().GetStorageNames().c_str());
            if (ImGui::Button("Add Component"))
            {
                auto cIt = GameMaster::Get().GetStorage().begin();
                advance(cIt, type);
                this->AddComponent(cIt->second());
            }

            ImGui::SameLine();
        }
    }

    ImGui::PopID();
}

json GameObject::Serialize()
{
    json gameObject;
    gameObject["name"] = name;
    gameObject["id"] = id;
    gameObject["transform"]["position"]["x"] = transform.position.x;
    gameObject["transform"]["position"]["y"] = transform.position.y;
    gameObject["transform"]["angle"] = transform.angle;
    auto copyComponents = this->components;
    int i = 0;
    for (auto component : copyComponents)
    {
        gameObject["components"][i] = component->Serialize();
        i++;
    }
    return gameObject;
}

void GameObject::Deserialize(json j)
{
    name = j["name"];
    id = j["id"];
    transform.position.x = j["transform"]["position"]["x"];
    transform.position.y = j["transform"]["position"]["y"];
    transform.angle = j["transform"]["angle"];
    auto jsonComponents = j["components"];
    for (auto jsonComponent : jsonComponents)
    {
        auto component = GameMaster::Get().GetComponent(jsonComponent);
        this->AddComponent(component);
        component->Deserialize(jsonComponent);
    }
}