#include "Component.h"
using namespace eng;

//Using only when component adding to gameobject by gameobject class
void Component::SetParent(GameObject * parent) {
    this->parent = parent;
}

void Component::Update() {

}

void Component::FixedUpdate() {

}