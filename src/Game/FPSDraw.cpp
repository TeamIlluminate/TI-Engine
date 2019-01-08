#include "Utils.h"
#include "Component.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include <algorithm>

namespace eng
{

class FPSDraw : public Component
{
    public:

    float time = 0;
    int fps = 0;
    float fpsCounter = 0;
    float fpsTime = 0;
    int objectsCount = 0;

    void Update()
    {
        time += DeltaTime();
        fpsTime += DeltaTime();
    }

    
shared_ptr<Component> Clone() {
    return make_shared<FPSDraw>(*this);
}

    void GUI()
    {
        fpsCounter++;

        if(fpsTime >= 1)
        { 
            fps = fpsCounter;
            fpsCounter = 0;
            fpsTime = 0;
        }

        if(auto owner = _owner.lock())
        {
            objectsCount = owner->GetScene().lock()->GetGameObjects().size();

            ImGui::Begin("Stats");

            ImGui::SetWindowPos(ImVec2(1500, 10));
            ImGui::SetWindowSize(ImVec2(400, 600));

            string _fps = to_string(fps);
            string _dTime = to_string(DeltaTime());
            string _gameTime = to_string(time);
            string _objectCount = to_string(objectsCount);

            ImGui::LabelText("<---FPS", _fps.c_str());
            ImGui::LabelText("<---DeltaTime", _dTime.c_str());
            ImGui::LabelText("<---Game time", _gameTime.c_str());
            ImGui::LabelText("<---Objects count", _objectCount.c_str());
            ImGui::Columns(2, "ObjectList", true);
            ImGui::Text("Object Name"); ImGui::NextColumn();
            ImGui::Text("Object Position"); ImGui::NextColumn();
            ImGui::Separator();


            for(auto _gameObject : owner->GetScene().lock()->GetGameObjects())
            {
                if(auto gameObject = _gameObject.lock())
                {        
                    ImGui::Text(gameObject->GetName().c_str());   
                    ImGui::NextColumn();
                    DrawVector2(gameObject->transform.position);
                    ImGui::NextColumn();
                    ImGui::Separator();
                }
            }
            ImGui::End();
        }
    }
};

}