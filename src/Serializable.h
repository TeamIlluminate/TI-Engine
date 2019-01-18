#include "Utils.h"
#pragma once

namespace eng
{

#define REGISTRY_COMPONENT(TYPE) if(Serializable::VerifyRegister(typeid(TYPE).name())) \
    Serializable::RegisterComponentConstructor(typeid(TYPE).name(), []() -> shared_ptr<Component> { \
    return make_shared<TYPE>(); \
     });
    
class Component;
static map<string, shared_ptr<Component> (*)()> constructorStorage;
//component
class Serializable
{
  public:
    virtual json Serialize()
    {
        json j;
        return j;
    };
    virtual void Deserialize(json s){};

    static shared_ptr<Component> GetComponent(json component);

    static bool VerifyRegister(string name);

    static void RegisterComponentConstructor(string typeName, shared_ptr<Component> (*cc)());
};
} // namespace eng