#include "Utils.h"
#include "Component.h"
#include "imgui.h"
#include "imgui-SFML.h"

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

    void GUI()
    {
        fpsCounter++;

        if(fpsTime >= 1)
        { 
            fps = fpsCounter;
            fpsCounter = 0;
            fpsTime = 0;
        }

        if(auto parent = owner.lock())
        {
            objectsCount = parent->GetScene()->GetGameObjects().size();
        }

        ImGui::Begin("Stats");

        ImGui::SetWindowSize(ImVec2(400, 150));

        string _fps = to_string(fps);
        string _dTime = to_string(DeltaTime());
        string _gameTime = to_string(time);
        string _objectCount = to_string(objectsCount);

        ImGui::LabelText("<---FPS", _fps.c_str());
        ImGui::LabelText("<---DeltaTime", _dTime.c_str());
        ImGui::LabelText("<---Game time", _gameTime.c_str());
        ImGui::LabelText("<---Objects count", _objectCount.c_str());

        ImGui::End();
    }
};

}