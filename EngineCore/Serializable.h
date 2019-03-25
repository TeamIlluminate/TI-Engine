#include "Utils.h"
#pragma once

namespace eng
{

    template<typename Var>
    auto DeserializeVar(Var &var, json obj)
    {
        try
        {
            var = obj;
        } 
        catch(exception)
        {
            // Нет нужды уведомлять о том, что некоторый параметр не смог десериализоваться - понятно всегда, почему и понятно будет что.
            // Максимум вывод в терминал можно сделать.
        }
    }

class Component;

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
};
} // namespace eng