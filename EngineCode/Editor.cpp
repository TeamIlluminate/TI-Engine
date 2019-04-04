#include "Editor.h"
#include <TI-EngineCore/API.h>
#include <dlfcn.h>

using namespace edr;

typedef std::function<void(shared_ptr<eng::Grid>)> voidGridDelegate;

Editor::Editor()
{
    editorWindow = make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "TI Engine improved");
}

void Editor::Start()
{
    LoadEditorLib();
    EditorDrawingCycle();
}

void Editor::EditorDrawingCycle()
{
    ImGui::SFML::Init(*editorWindow);
    sf::Clock imguiTick;

    while (editorWindow->isOpen())
    {
        editorWindow->clear();

        sf::Event event;
        while (editorWindow->pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                editorWindow->close();
        }

        ImGui::SFML::Update(*editorWindow, imguiTick.restart());

        if (draw)
        {
            //Draw here Editor GUI
            auto EGS = editorGuiStorage;
            for (auto grid : EGS)
            {
                grid->Draw();
            }
        }

        ImGui::SFML::Render(*editorWindow);

        editorWindow->display();
    }

    ImGui::SFML::Shutdown();
}

void Editor::LoadEditorLib()
{
    // Stop GUI drawing, clear GUI and free lib(if it's initialized)
    draw = false;
    editorGuiStorage.clear();

    if (editorLib)
        dlclose(*editorLib.get());

    void (*rld)(voidDelegate);     // Reload Handler setter structure
    void (*grd)(voidGridDelegate); // Grid argument structure
    void (*init)();                // Init structure
    char *error;

    editorLib = make_shared<void *>(dlopen("libTIEditor.so", RTLD_LAZY));

    if (!editorLib)
    {
        cout << "cnt load lib\n";
        cerr << dlerror();
        return;
    }

    // First of all -> link to init function

    init = (void (*)())dlsym(*editorLib.get(), "InitEditorLib"); // Gets address of Init lib function

    if ((error = dlerror()) != NULL)
    {
        cout << "cnt load InitEditor\n";
        cerr << error;
        return;
    }

    // Link & set reload handler

    rld = (void (*)(voidDelegate))dlsym(*editorLib.get(), "SetReloadHandler");

    if ((error = dlerror()) != NULL)
    {
        cout << "cnt load SetReload\n";
        cerr << error;
        return;
    }

    rld(bind(&Editor::LoadEditorLib, this));

    // Set GUI Grids handlers

    grd = (void (*)(voidGridDelegate))dlsym(*editorLib.get(), "SetAddGridHandler");

    if ((error = dlerror()) != NULL)
    {
        cout << "cnt load SetAdd\n";
        cerr << error;
        return;
    }

    grd(bind(&Editor::AddGrid, this, placeholders::_1));

    grd = (void (*)(voidGridDelegate))dlsym(*editorLib.get(), "SetRemoveGridHandler");

    if ((error = dlerror()) != NULL)
    {
        cout << "cnt load SetRemove\n";
        cerr << error;
        return;
    }

    grd(bind(&Editor::RemoveGrid, this, placeholders::_1));

    //Now init lib.
    init();
    draw = true;
}

void Editor::AddGrid(shared_ptr<eng::Grid> grid)
{
    editorGuiStorage.push_back(grid);
}

void Editor::RemoveGrid(shared_ptr<eng::Grid> grid)
{
    editorGuiStorage.remove(grid);
}